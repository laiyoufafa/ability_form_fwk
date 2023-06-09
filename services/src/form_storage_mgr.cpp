/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "form_storage_mgr.h"

#include <cinttypes>
#include <cstdio>
#include <dirent.h>
#include <fstream>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "form_constants.h"
#include "form_mgr_errors.h"
#include "form_storage_mgr.h"
#include "hilog_wrapper.h"
#include "kvstore_death_recipient_callback.h"
#include "securec.h"
#include "string_ex.h"
#include "types.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
const int32_t MAX_TIMES = 600;              // 1min
const int32_t SLEEP_INTERVAL = 100 * 1000;  // 100ms
}  // namespace

FormStorageMgr::FormStorageMgr()
{
    HILOG_INFO("instance is created");
    TryTwice([this] { return GetKvStore(); });
    RegisterKvStoreDeathListener();
}

FormStorageMgr::~FormStorageMgr()
{
    HILOG_INFO("instance is destroyed");
    dataManager_.CloseKvStore(appId_, kvStorePtr_);
}

void FormStorageMgr::SaveEntries(
    const std::vector<DistributedKv::Entry> &allEntries, std::vector<InnerFormInfo> &innerFormInfos)
{
    for (const auto &item : allEntries) {
        std::string formId;
        InnerFormInfo innerFormInfo;

        nlohmann::json jsonObject = nlohmann::json::parse(item.value.ToString(), nullptr, false);
        if (jsonObject.is_discarded()) {
            HILOG_ERROR("error key: %{private}s", item.key.ToString().c_str());
            // it's an bad json, delete it
            {
                std::lock_guard<std::mutex> lock(kvStorePtrMutex_);
                kvStorePtr_->Delete(item.key);
            }
            continue;
        }
        if (innerFormInfo.FromJson(jsonObject) != true) {
            HILOG_ERROR("error key: %{private}s", item.key.ToString().c_str());
            // it's an error value, delete it
            {
                std::lock_guard<std::mutex> lock(kvStorePtrMutex_);
                kvStorePtr_->Delete(item.key);
            }
            continue;
        }

        if (std::find(innerFormInfos.begin(), innerFormInfos.end(), innerFormInfo) == innerFormInfos.end()) {
            HILOG_DEBUG("emplace FormInfos: %{public}s", formId.c_str());
            std::map<std::string, InnerFormInfo> allDevicesInfos;
            allDevicesInfos.emplace(formId, innerFormInfo);
            innerFormInfos.emplace_back(innerFormInfo);
        }
    }
    HILOG_DEBUG("SaveEntries end");
}

/**
 * @brief Load all form data from DB to innerFormInfos.
 * @param innerFormInfos Storage all form data.
 * @return Returns ERR_OK on success, others on failure.
 */
ErrCode FormStorageMgr::LoadFormData(std::vector<InnerFormInfo> &innerFormInfos)
{
    HILOG_INFO("%{public}s called.", __func__);
    bool ret = ERR_OK;
    {
        std::lock_guard<std::mutex> lock(kvStorePtrMutex_);
        if (!CheckKvStore()) {
            HILOG_ERROR("kvStore is nullptr");
            return ERR_APPEXECFWK_FORM_COMMON_CODE;
        }
    }
    DistributedKv::Status status;
    std::vector<DistributedKv::Entry> allEntries;
    TryTwice([this, &status, &allEntries] {
        status = GetEntries(allEntries);
        return status;
    });

    if (status != DistributedKv::Status::SUCCESS) {
        HILOG_ERROR("get entries error: %{public}d", status);
        ret = ERR_APPEXECFWK_FORM_COMMON_CODE;
    } else {
        SaveEntries(allEntries, innerFormInfos);
    }

    HILOG_INFO("%{public}s, readdir over", __func__);
    return ret;
}

/**
 * @brief Get form data from DB to innerFormInfo with formId.
 * @param innerFormInfo Storage form data.
 * @return Returns ERR_OK on success, others on failure.
 */
ErrCode FormStorageMgr::GetStorageFormInfoById(const std::string &formId, InnerFormInfo &innerFormInfo)
{
    ErrCode ret = ERR_OK;
    HILOG_DEBUG("%{public}s called, formId[%{public}s]", __func__, formId.c_str());

    {
        std::lock_guard<std::mutex> lock(kvStorePtrMutex_);
        if (!CheckKvStore()) {
            HILOG_ERROR("kvStore is nullptr");
            return ERR_APPEXECFWK_FORM_COMMON_CODE;
        }
    }

    DistributedKv::Status status = DistributedKv::Status::ERROR;
    std::vector<DistributedKv::Entry> allEntries;
    DistributedKv::Key key(formId);
    if (kvStorePtr_) {
        // sync call GetEntries, the callback will be trigger at once
        status = kvStorePtr_->GetEntries(key, allEntries);
    }

    if (status != DistributedKv::Status::SUCCESS) {
        HILOG_ERROR("get entries error: %{public}d", status);
        ret = ERR_APPEXECFWK_FORM_COMMON_CODE;
    } else {
        if (allEntries.empty()) {
            HILOG_ERROR("%{public}s not match any FormInfo", formId.c_str());
            ret = ERR_APPEXECFWK_FORM_COMMON_CODE;
        } else {
            nlohmann::json jsonObject = nlohmann::json::parse(allEntries.front().value.ToString(), nullptr, false);
            if (jsonObject.is_discarded()) {
                HILOG_ERROR("error key: %{private}s", allEntries.front().key.ToString().c_str());
                ret = ERR_APPEXECFWK_FORM_COMMON_CODE;
            }
            if (innerFormInfo.FromJson(jsonObject) != true) {
                HILOG_ERROR("error key: %{private}s", allEntries.front().key.ToString().c_str());
                ret = ERR_APPEXECFWK_FORM_COMMON_CODE;
            }
        }
    }

    return ret;
}

/**
 * @brief Save or update the form data in DB.
 * @param innerFormInfo Indicates the InnerFormInfo object to be save.
 * @return Returns ERR_OK on success, others on failure.
 */
ErrCode FormStorageMgr::SaveStorageFormInfo(const InnerFormInfo &innerFormInfo)
{
    HILOG_INFO("%{public}s called, formId[%{public}" PRId64 "]", __func__, innerFormInfo.GetFormId());
    ErrCode ret = ERR_OK;
    std::string formId = std::to_string(innerFormInfo.GetFormId());

    {
        std::lock_guard<std::mutex> lock(kvStorePtrMutex_);
        if (!CheckKvStore()) {
            HILOG_ERROR("kvStore is nullptr");
            return ERR_APPEXECFWK_FORM_COMMON_CODE;
        }
    }

    DistributedKv::Key key(formId);
    DistributedKv::Value value(innerFormInfo.ToString());
    DistributedKv::Status status;
    {
        std::lock_guard<std::mutex> lock(kvStorePtrMutex_);
        status = kvStorePtr_->Put(key, value);
        if (status == DistributedKv::Status::IPC_ERROR) {
            status = kvStorePtr_->Put(key, value);
            HILOG_WARN("distribute database ipc error and try to call again, result = %{public}d", status);
        }
    }
    if (status != DistributedKv::Status::SUCCESS) {
        HILOG_ERROR("put innerFormInfo to kvStore error: %{public}d", status);
        ret = ERR_APPEXECFWK_FORM_COMMON_CODE;
    }
    return ret;
}

/**
 * @brief Modify the form data in DB.
 * @param innerFormInfo Indicates the InnerFormInfo object to be Modify.
 * @return Returns ERR_OK on success, others on failure.
 */
ErrCode FormStorageMgr::ModifyStorageFormInfo(const InnerFormInfo &innerFormInfo)
{
    HILOG_INFO("%{public}s called, formId[%{public}" PRId64 "]", __func__, innerFormInfo.GetFormId());

    ErrCode ret = ERR_OK;
    std::string formId = std::to_string(innerFormInfo.GetFormId());
    ret = DeleteStorageFormInfo(formId);
    if (ret == ERR_OK) {
        SaveStorageFormInfo(innerFormInfo);
    }

    return ret;
}

/**
 * @brief Delete the form data in DB.
 * @param formId The form data Id.
 * @return Returns ERR_OK on success, others on failure.
 */
ErrCode FormStorageMgr::DeleteStorageFormInfo(const std::string &formId)
{
    HILOG_INFO("%{public}s called, formId[%{public}s]", __func__, formId.c_str());

    {
        std::lock_guard<std::mutex> lock(kvStorePtrMutex_);
        if (!CheckKvStore()) {
            HILOG_ERROR("kvStore is nullptr");
            return ERR_APPEXECFWK_FORM_COMMON_CODE;
        }
    }
    DistributedKv::Key key(formId);
    DistributedKv::Status status;

    {
        std::lock_guard<std::mutex> lock(kvStorePtrMutex_);
        status = kvStorePtr_->Delete(key);
        if (status == DistributedKv::Status::IPC_ERROR) {
            status = kvStorePtr_->Delete(key);
            HILOG_WARN("distribute database ipc error and try to call again, result = %{public}d", status);
        }
    }

    if (status != DistributedKv::Status::SUCCESS) {
        HILOG_ERROR("delete key error: %{public}d", status);
        return ERR_APPEXECFWK_FORM_COMMON_CODE;
    } else {
        HILOG_ERROR("delete value to kvStore success");
    }

    return ERR_OK;
}

void FormStorageMgr::RegisterKvStoreDeathListener()
{
    HILOG_INFO("register kvStore death listener");
    std::shared_ptr<DistributedKv::KvStoreDeathRecipient> callback =
    std::make_shared<KvStoreDeathRecipientCallback>();
    dataManager_.RegisterKvStoreServiceDeathRecipient(callback);
}

bool FormStorageMgr::CheckKvStore()
{
    if (kvStorePtr_ != nullptr) {
        return true;
    }
    int32_t tryTimes = MAX_TIMES;
    while (tryTimes > 0) {
        DistributedKv::Status status = GetKvStore();
        if (status == DistributedKv::Status::SUCCESS && kvStorePtr_ != nullptr) {
            return true;
        }
        HILOG_DEBUG("CheckKvStore, Times: %{public}d", tryTimes);
        usleep(SLEEP_INTERVAL);
        tryTimes--;
    }
    return kvStorePtr_ != nullptr;
}

DistributedKv::Status FormStorageMgr::GetKvStore()
{
    DistributedKv::Options options = {
        .createIfMissing = true,
        .encrypt = false,
        .autoSync = true,
        .kvStoreType = DistributedKv::KvStoreType::SINGLE_VERSION,
        .baseDir = Constants::FORM_STORAGE_DIR,
        .area = DistributedKv::EL1
        };

    DistributedKv::Status status = dataManager_.GetSingleKvStore(options, appId_, storeId_, kvStorePtr_);
    if (status != DistributedKv::Status::SUCCESS) {
        HILOG_ERROR("return error: %{public}d", status);
    } else {
        HILOG_INFO("get kvStore success");
    }
    return status;
}

DistributedKv::Status FormStorageMgr::GetEntries(std::vector<DistributedKv::Entry> &allEntries)
{
    DistributedKv::Status status = DistributedKv::Status::ERROR;
    // if prefix is empty, get all entries.
    DistributedKv::Key key("");
    if (kvStorePtr_) {
        // sync call GetEntries, the callback will be trigger at once
        status = kvStorePtr_->GetEntries(key, allEntries);
    }
    HILOG_INFO("get all entries status: %{public}d", status);
    return status;
}

void FormStorageMgr::TryTwice(const std::function<DistributedKv::Status()> &func)
{
    DistributedKv::Status status = func();
    if (status == DistributedKv::Status::IPC_ERROR) {
        status = func();
        HILOG_WARN("distribute database ipc error and try to call again, result = %{public}d", status);
    }
}

bool FormStorageMgr::ResetKvStore()
{
    std::lock_guard<std::mutex> lock(kvStorePtrMutex_);
    kvStorePtr_ = nullptr;
    DistributedKv::Status status = GetKvStore();
    if (status == DistributedKv::Status::SUCCESS && kvStorePtr_ != nullptr) {
        return true;
    }
    HILOG_WARN("failed");
    return false;
}
}  // namespace AppExecFwk
}  // namespace OHOS
