/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_FORM_FWK_FORM_DATA_PROXY_RECORD_H
#define OHOS_FORM_FWK_FORM_DATA_PROXY_RECORD_H

#include <vector>

#include "datashare_helper.h"
#include "form_provider_data_proxy.h"

namespace OHOS {
namespace AppExecFwk {
/**
 * @class FormDataSharerRecord
 * data share form record.
 */
class FormDataProxyRecord : public std::enable_shared_from_this<FormDataProxyRecord> {
public:
    FormDataProxyRecord(int64_t formId, const std::string &bundleName, const std::string &moduleName);
    ~FormDataProxyRecord();

    ErrCode SubscribeFormData(const std::vector<FormDataProxy> &formDataProxies);
    ErrCode UnsubscribeFormData();
private:
    struct FormDataProxyRequest {
        int64_t subscribeId;
        std::vector<std::string> uris;
    };

    void ParseFormDataProxies(const std::vector<FormDataProxy> &formDataProxies);
    void ConvertSubscribeMapToRequests(std::map<std::string, std::string> &subscribeMap,
        std::vector<FormDataProxyRequest> &formDataProxyRequests);
    void UpdatePublishedDataForm(const std::vector<DataShare::PublishedDataItem> &data);
    void UpdateRdbDataForm(const std::vector<std::string> &data);
    ErrCode SubscribeRdbFormData();
    ErrCode SubscribePublishFormData();

    void OnRdbDataChange(const DataShare::RdbChangeNode &changeNode);
    void OnPublishedDataChange(const DataShare::PublishedDataChangeNode &changeNode);

    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper_;
    int64_t formId_ = -1;
    std::string bundleName_;
    std::string moduleName_;
    std::map<std::string, std::string> rdbSubscribeMap_; // key: subscribeId
    std::map<std::string, std::string> publishSubscribeMap_; // key: subscribeId
};
} // namespace AppExecFwk
} // namespace OHOS

#endif // OHOS_FORM_FWK_FORM_DATA_PROXY_RECORD_H