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

#include "form_bms_helper.h"
#include "ability_manager_interface.h"
#include "appexecfwk_errors.h"
#include "hilog_wrapper.h"
#include "if_system_ability_manager.h"
#include "in_process_call_wrapper.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "system_ability_definition.h"

namespace OHOS {
namespace AppExecFwk {
FormBmsHelper::FormBmsHelper()
{}

FormBmsHelper::~FormBmsHelper()
{}

/**
 * @brief Acquire a bundle manager, if it not existed.
 * @return returns the bundle manager ipc object, or nullptr for failed.
 */
sptr<IBundleMgr> FormBmsHelper::GetBundleMgr()
{
    HILOG_INFO("%{public}s called.", __func__);

    if (iBundleMgr_ == nullptr) {
        sptr<ISystemAbilityManager> systemAbilityManager =
        SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
        auto remoteObject = systemAbilityManager->GetSystemAbility(BUNDLE_MGR_SERVICE_SYS_ABILITY_ID);
        if (remoteObject == nullptr) {
            HILOG_ERROR("%{public}s error, failed to get bundle manager service.", __func__);
            return nullptr;
        }

        iBundleMgr_ = iface_cast<IBundleMgr>(remoteObject);
        if (iBundleMgr_ == nullptr) {
            HILOG_ERROR("%{public}s error, failed to get bundle manager service", __func__);
            return nullptr;
        }
    }
    return iBundleMgr_;
}

/**
 * @brief Add the bundle manager instance for debug.
 * @param bundleManager the bundle manager ipc object.
 */
void FormBmsHelper::SetBundleManager(const sptr<IBundleMgr> &bundleManager)
{
    HILOG_INFO("%{public}s called.", __func__);

    iBundleMgr_ = bundleManager;
}
/**
 * @brief Notify module removable.
 * @param bundleName Provider ability bundleName.
 * @param moduleName Provider ability moduleName.
 */
void FormBmsHelper::NotifyModuleRemovable(const std::string &bundleName, const std::string &moduleName)
{
    HILOG_INFO("%{public}s, bundleName:%{public}s, moduleName:%{public}s",
        __func__, bundleName.c_str(), moduleName.c_str());
    if (bundleName.empty() || moduleName.empty()) {
        return;
    }

    std::string key = GenerateModuleKey(bundleName, moduleName);
    HILOG_INFO("%{public}s, begin to notify %{public}s removable", __func__, key.c_str());
    sptr<IBundleMgr> iBundleMgr = GetBundleMgr();
    if (iBundleMgr == nullptr) {
        HILOG_ERROR("%{public}s, failed to get IBundleMgr.", __func__);
        return;
    }

    std::string originId = IPCSkeleton::ResetCallingIdentity();

    IPCSkeleton::SetCallingIdentity(originId);
}
/**
 * @brief Notify module not removable.
 * @param bundleName Provider ability bundleName.
 * @param moduleName Provider ability moduleName.
 */
void FormBmsHelper::NotifyModuleNotRemovable(const std::string &bundleName, const std::string &moduleName)
{
    std::string key = GenerateModuleKey(bundleName, moduleName);
    HILOG_INFO("%{public}s, begin to notify %{public}s not removable", __func__, key.c_str());
    sptr<IBundleMgr> iBundleMgr = GetBundleMgr();
    if (iBundleMgr == nullptr) {
        HILOG_ERROR("%{public}s, failed to get IBundleMgr.", __func__);
        return;
    }

    if (!IN_PROCESS_CALL(iBundleMgr->SetModuleRemovable(bundleName, moduleName, false))) {
        HILOG_ERROR("%{public}s, set not removable failed.", __func__);
        return;
    }
    return;
}

std::string FormBmsHelper::GenerateModuleKey(const std::string &bundleName, const std::string &moduleName) const
{
    return bundleName + "#" + moduleName;
}

bool FormBmsHelper::GetBundlePackInfo(const std::string &bundleName, const int32_t userId,
    BundlePackInfo &bundlePackInfo)
{
    HILOG_INFO("%{public}s, bundleName:%{public}s", __func__, bundleName.c_str());
    sptr<IBundleMgr> iBundleMgr = GetBundleMgr();
    if (iBundleMgr == nullptr) {
        HILOG_ERROR("%{public}s, failed to get IBundleMgr.", __func__);
        return false;
    }

    if (!IN_PROCESS_CALL(iBundleMgr->GetBundlePackInfo(bundleName, GET_PACK_INFO_ALL, bundlePackInfo, userId))) {
        HILOG_ERROR("%{public}s error, failed to get bundle pack info.", __func__);
        return false;
    }

    HILOG_INFO("%{public}s, get bundle pack info success", __func__);
    return true;
}
}  // namespace AppExecFwk
}  // namespace OHOS
