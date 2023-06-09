
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

#include "form_ability_connection.h"

#include <cinttypes>

#include "appexecfwk_errors.h"
#include "form_bms_helper.h"
#include "form_data_mgr.h"
#include "form_db_cache.h"
#include "form_info.h"
#include "form_info_mgr.h"
#include "form_supply_callback.h"
#include "form_task_mgr.h"
#include "form_timer_mgr.h"
#include "hilog_wrapper.h"
#include "ipc_types.h"
#include "message_parcel.h"
#include "want.h"

namespace OHOS {
namespace AppExecFwk {
/**
 * @brief OnAbilityConnectDone, AbilityMs notify caller ability the result of connect.
 * @param element service ability's ElementName.
 * @param remoteObject the session proxy of service ability.
 * @param resultCode ERR_OK on success, others on failure.
 */
void FormAbilityConnection::OnAbilityConnectDone(
    const AppExecFwk::ElementName &element, const sptr<IRemoteObject> &remoteObject, int resultCode)
{
    if (resultCode != ERR_OK) {
        HILOG_ERROR("%{public}s, formId:%{public}" PRId64 ", resultCode:%{public}d",
        __func__, formId_, resultCode);
        return;
    }

    HILOG_INFO("%{public}s, free install is %{public}d.", __func__, isFreeInstall_);
    if (!isFreeInstall_) {
        return;
    }

    std::vector<FormInfo> targetForms;
    if (FormInfoMgr::GetInstance().GetFormsInfoByBundle(bundleName_, targetForms) != ERR_OK) {
        HILOG_ERROR("%{public}s error, failed to get forms info.", __func__);
        return;
    }

    FormRecord formRecord;
    if (!FormDataMgr::GetInstance().GetFormRecord(formId_, formRecord)) {
        HILOG_ERROR("%{public}s error, not exist such form:%{public}" PRId64 ".", __func__, formId_);
        return;
    }

    FormDataMgr::GetInstance().SetRecordNeedFreeInstall(formId_, false);
    FormInfo updatedForm;
    if (FormDataMgr::GetInstance().GetUpdatedForm(formRecord, targetForms, updatedForm)) {
        HILOG_INFO("%{public}s, refresh form", __func__);
        FormDataMgr::GetInstance().SetNeedRefresh(formId_, true);
        FormBmsHelper::GetInstance().NotifyModuleNotRemovable(formRecord.bundleName, formRecord.moduleName);
        return;
    }

    // delete form
    if (formRecord.formTempFlg) {
        FormDataMgr::GetInstance().DeleteTempForm(formId_);
    } else {
        FormDbCache::GetInstance().DeleteFormInfo(formId_);
    }
    FormDataMgr::GetInstance().DeleteFormRecord(formId_);
    const std::vector<int64_t> removedForms {formId_};
    FormDataMgr::GetInstance().CleanHostRemovedForms(removedForms);
    FormTimerMgr::GetInstance().RemoveFormTimer(formId_);
}

/**
 * @brief OnAbilityDisconnectDone, AbilityMs notify caller ability the result of disconnect.
 * @param element service ability's ElementName.
 * @param resultCode ERR_OK on success, others on failure.
 */
void FormAbilityConnection::OnAbilityDisconnectDone(const AppExecFwk::ElementName &element, int resultCode)
{
    HILOG_DEBUG(
        "%{public}s, element:%{public}s, resultCode:%{public}d", __func__, element.GetURI().c_str(), resultCode);
    if (connectId_ != 0) {
        FormSupplyCallback::GetInstance()->RemoveConnection(connectId_);
        connectId_ = 0;
    } else {
        HILOG_ERROR("%{public}s fail, connectId_ invalidate. connectId_: %{public}ld", __func__, connectId_);
    }
}

/**
 * @brief Remote object died event.
 * @param remoteObject the remote object of service ability.
 */
void FormAbilityConnection::OnConnectDied(const wptr<IRemoteObject> &remoteObject)
{
    if (connectId_ != 0) {
        FormSupplyCallback::GetInstance()->RemoveConnection(connectId_);
        connectId_ = 0;
    } else {
        HILOG_ERROR("%{public}s fail, connectId_ invalidate. connectId_: %{public}ld", __func__, connectId_);
    }
}

/**
 * @brief Set connectId.
 * @param connectId The ability connection id.
 */
void FormAbilityConnection::SetConnectId(long connectId)
{
    HILOG_INFO("%{public}s, connectId_: %{public}ld", __func__, connectId);
    connectId_ = connectId;
}

/**
 * @brief Get connectId.
 * @return The ability connection id.
 */
long FormAbilityConnection::GetConnectId()
{
    return connectId_;
}

/**
 * @brief Get the provider Key
 *
 * @return The provider Key
 */
std::string FormAbilityConnection::GetProviderKey()
{
    if (bundleName_.empty() || abilityName_.empty()) {
        return "";
    }
    return bundleName_ + "::" + abilityName_;
}

/**
 * @brief Set the Provider Key
 *
 * @param bundleName bundleName
 * @param abilityName abilityName
 */
void FormAbilityConnection::SetProviderKey(const std::string &bundleName, const std::string &abilityName)
{
    bundleName_ = bundleName;
    abilityName_ = abilityName;
}

void FormAbilityConnection::SetFreeInstall(bool isFreeInstall)
{
    isFreeInstall_ = isFreeInstall;
}

void FormAbilityConnection::SetFormId(int64_t formId)
{
    formId_ = formId;
}

int64_t FormAbilityConnection::GetFormId() const
{
    return formId_;
}
}  // namespace AppExecFwk
}  // namespace OHOS