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
#include <chrono>
#include <gtest/gtest.h>

#include "appexecfwk_errors.h"
#define private public
#define protected public
#include "form_mgr_proxy.h"
#undef private
#undef protected
#include "form_constants.h"
#include "form_mgr_errors.h"
#include "message_parcel.h"
#include "mock_i_remote_object.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::AppExecFwk;
using namespace std::placeholders;

extern void MockWriteInterfaceToken(bool mockRet);

namespace {
class FmsFormMgrProxyTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

protected:
};

void FmsFormMgrProxyTest::SetUpTestCase() {}
void FmsFormMgrProxyTest::TearDownTestCase() {}
void FmsFormMgrProxyTest::SetUp()
{
    MockWriteInterfaceToken(true);
}
void FmsFormMgrProxyTest::TearDown() {}

int SendRequestReplace(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option,
    int32_t error, bool setError, bool retBool, bool setRetBool)
{
    if (setError) {
        reply.WriteInt32(error);
    }
    if (setRetBool) {
        reply.WriteBool(retBool);
    }
    return 0;
}

/*
 * @tc.name: AddFormTest_0100
 * @tc.desc: test AddForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, AddFormTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, AddFormTest_0100, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    MockWriteInterfaceToken(false);
    Want want;
    FormJsInfo formInfo;
    int32_t result = proxy->AddForm(0, want, nullptr, formInfo);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: AddFormTest_0200
 * @tc.desc: test AddForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, AddFormTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, AddFormTest_0100, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(ERR_OK)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    FormJsInfo formInfo;
    int32_t result = proxy->AddForm(0, want, nullptr, formInfo);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: DeleteFormTest_0100
 * @tc.desc: test DeleteForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, DeleteFormTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, DeleteFormTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->DeleteForm(0, nullptr);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: DeleteFormTest_0200
 * @tc.desc: test DeleteForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, DeleteFormTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, DeleteFormTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->DeleteForm(0, nullptr);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: DeleteFormTest_0300
 * @tc.desc: test DeleteForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, DeleteFormTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, DeleteFormTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->DeleteForm(0, nullptr);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: ReleaseFormTest_0100
 * @tc.desc: test ReleaseForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, ReleaseFormTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, ReleaseFormTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->ReleaseForm(0, nullptr, true);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: ReleaseFormTest_0200
 * @tc.desc: test ReleaseForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, ReleaseFormTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, ReleaseFormTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->ReleaseForm(0, nullptr, true);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: ReleaseFormTest_0300
 * @tc.desc: test ReleaseForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, ReleaseFormTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, ReleaseFormTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->ReleaseForm(0, nullptr, true);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: UpdateFormTest_0100
 * @tc.desc: test UpdateForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, UpdateFormTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, UpdateFormTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    FormProviderData FormProviderData;
    int32_t result = proxy->UpdateForm(0, FormProviderData);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: UpdateFormTest_0200
 * @tc.desc: test UpdateForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, UpdateFormTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, UpdateFormTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    FormProviderData FormProviderData;
    int32_t result = proxy->UpdateForm(0, FormProviderData);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: UpdateFormTest_0300
 * @tc.desc: test UpdateForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, UpdateFormTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, UpdateFormTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    FormProviderData FormProviderData;
    int32_t result = proxy->UpdateForm(0, FormProviderData);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: SetNextRefreshTimeTest_0100
 * @tc.desc: test SetNextRefreshTime function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, SetNextRefreshTimeTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, SetNextRefreshTimeTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->SetNextRefreshTime(0, 0);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: SetNextRefreshTimeTest_0200
 * @tc.desc: test SetNextRefreshTime function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, SetNextRefreshTimeTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, SetNextRefreshTimeTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->SetNextRefreshTime(0, 0);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: SetNextRefreshTimeTest_0300
 * @tc.desc: test SetNextRefreshTime function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, SetNextRefreshTimeTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, SetNextRefreshTimeTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->SetNextRefreshTime(0, 0);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: LifecycleUpdateTest_0100
 * @tc.desc: test LifecycleUpdate function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, LifecycleUpdateTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, LifecycleUpdateTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    std::vector<int64_t> formIds{0};
    int32_t result = proxy->LifecycleUpdate(formIds, nullptr, true);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: LifecycleUpdateTest_0200
 * @tc.desc: test LifecycleUpdate function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, LifecycleUpdateTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, LifecycleUpdateTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    std::vector<int64_t> formIds{0};
    int32_t result = proxy->LifecycleUpdate(formIds, nullptr, true);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: LifecycleUpdateTest_0300
 * @tc.desc: test LifecycleUpdate function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, LifecycleUpdateTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, LifecycleUpdateTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    std::vector<int64_t> formIds{0};
    int32_t result = proxy->LifecycleUpdate(formIds, nullptr, true);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: RequestFormTest_0100
 * @tc.desc: test RequestForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, RequestFormTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, RequestFormTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    int32_t result = proxy->RequestForm(0, nullptr, want);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: RequestFormTest_0200
 * @tc.desc: test RequestForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, RequestFormTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, RequestFormTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    int32_t result = proxy->RequestForm(0, nullptr, want);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: RequestFormTest_0300
 * @tc.desc: test RequestForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, RequestFormTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, RequestFormTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    int32_t result = proxy->RequestForm(0, nullptr, want);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: NotifyWhetherVisibleFormsTest_0100
 * @tc.desc: test NotifyWhetherVisibleForms function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, NotifyWhetherVisibleFormsTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, NotifyWhetherVisibleFormsTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    std::vector<int64_t> formIds{0};
    int32_t result = proxy->NotifyWhetherVisibleForms(formIds, nullptr, 0);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: NotifyWhetherVisibleFormsTest_0200
 * @tc.desc: test NotifyWhetherVisibleForms function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, NotifyWhetherVisibleFormsTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, NotifyWhetherVisibleFormsTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    std::vector<int64_t> formIds{0};
    int32_t result = proxy->NotifyWhetherVisibleForms(formIds, nullptr, 0);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: NotifyWhetherVisibleFormsTest_0300
 * @tc.desc: test NotifyWhetherVisibleForms function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, NotifyWhetherVisibleFormsTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, NotifyWhetherVisibleFormsTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    std::vector<int64_t> formIds{0};
    int32_t result = proxy->NotifyWhetherVisibleForms(formIds, nullptr, 0);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: CastTempFormTest_0100
 * @tc.desc: test CastTempForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, CastTempFormTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, CastTempFormTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->CastTempForm(0, nullptr);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: CastTempFormTest_0200
 * @tc.desc: test CastTempForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, CastTempFormTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, CastTempFormTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->CastTempForm(0, nullptr);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: CastTempFormTest_0300
 * @tc.desc: test CastTempForm function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, CastTempFormTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, CastTempFormTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    int32_t result = proxy->CastTempForm(0, nullptr);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: MessageEventTest_0100
 * @tc.desc: test MessageEvent function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, MessageEventTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, MessageEventTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    int32_t result = proxy->MessageEvent(0, want, nullptr);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: MessageEventTest_0200
 * @tc.desc: test MessageEvent function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, MessageEventTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, MessageEventTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    int32_t result = proxy->MessageEvent(0, want, nullptr);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: MessageEventTest_0300
 * @tc.desc: test MessageEvent function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, MessageEventTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, MessageEventTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    int32_t result = proxy->MessageEvent(0, want, nullptr);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: RouterEventTest_0100
 * @tc.desc: test RouterEvent function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, RouterEventTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, RouterEventTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    int32_t result = proxy->RouterEvent(0, want, nullptr);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: RouterEventTest_0200
 * @tc.desc: test RouterEvent function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, RouterEventTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, RouterEventTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    int32_t result = proxy->RouterEvent(0, want, nullptr);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: RouterEventTest_0300
 * @tc.desc: test RouterEvent function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, RouterEventTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, RouterEventTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    int32_t result = proxy->RouterEvent(0, want, nullptr);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: DeleteInvalidFormsTest_0100
 * @tc.desc: test DeleteInvalidForms function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, DeleteInvalidFormsTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, DeleteInvalidFormsTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    std::vector<int64_t> formIds{0};
    int32_t numFormsDeleted = 0;
    int32_t result = proxy->DeleteInvalidForms(formIds, nullptr, numFormsDeleted);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: DeleteInvalidFormsTest_0200
 * @tc.desc: test DeleteInvalidForms function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, DeleteInvalidFormsTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, DeleteInvalidFormsTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    std::vector<int64_t> formIds{0};
    int32_t numFormsDeleted = 0;
    int32_t result = proxy->DeleteInvalidForms(formIds, nullptr, numFormsDeleted);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: DeleteInvalidFormsTest_0300
 * @tc.desc: test DeleteInvalidForms function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, DeleteInvalidFormsTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, DeleteInvalidFormsTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    std::vector<int64_t> formIds{0};
    int32_t numFormsDeleted = 0;
    int32_t result = proxy->DeleteInvalidForms(formIds, nullptr, numFormsDeleted);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}

/*
 * @tc.name: AcquireFormStateTest_0100
 * @tc.desc: test AcquireFormState function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, AcquireFormStateTest_0100, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, AcquireFormStateTest_0100, TestSize.Level1";
    MockWriteInterfaceToken(false);
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    FormStateInfo stateInfo;
    int32_t result = proxy->AcquireFormState(want, nullptr, stateInfo);
    EXPECT_EQ(ERR_APPEXECFWK_PARCEL_ERROR, result);
}

/*
 * @tc.name: AcquireFormStateTest_0200
 * @tc.desc: test AcquireFormState function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, AcquireFormStateTest_0200, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, AcquireFormStateTest_0200, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _))
        .WillRepeatedly(DoAll(Invoke(std::bind(SendRequestReplace, _1, _2, _3, _4,
        ERR_OK, true, false, false)), Return(NO_ERROR)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    FormStateInfo stateInfo;
    int32_t result = proxy->AcquireFormState(want, nullptr, stateInfo);
    EXPECT_EQ(ERR_OK, result);
}

/*
 * @tc.name: AcquireFormStateTest_0300
 * @tc.desc: test AcquireFormState function
 * @tc.type: FUNC
 */
HWTEST_F(FmsFormMgrProxyTest, AcquireFormStateTest_0300, Function | MediumTest | Level1)
{
    GTEST_LOG_(INFO)
        << "FmsFormMgrProxyTest, AcquireFormStateTest_0300, TestSize.Level1";
    sptr<MockIRemoteObject> iremoteObject = new (std::nothrow) MockIRemoteObject();
    ASSERT_NE(nullptr, iremoteObject);
    EXPECT_CALL(*iremoteObject, SendRequest(_, _, _, _)).Times(1)
        .WillRepeatedly(DoAll(Return(-1)));
    std::shared_ptr<FormMgrProxy> proxy = std::make_shared<FormMgrProxy>(iremoteObject);
    ASSERT_NE(nullptr, proxy);
    Want want;
    FormStateInfo stateInfo;
    int32_t result = proxy->AcquireFormState(want, nullptr, stateInfo);
    EXPECT_EQ(ERR_APPEXECFWK_FORM_SEND_FMS_MSG, result);
}
}
