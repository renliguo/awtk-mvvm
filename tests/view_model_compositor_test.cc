﻿#include "gtest/gtest.h"
#include "tkc/object_default.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/base/view_model_compositor.h"
#include "humidity_view_model.h"
#include "temperature_view_model.h"

TEST(ViewModelCompositor, basic) {
  value_t v;
  view_model_factory_init();
  navigator_request_t req;
  memset(&req, 0x00, sizeof(req));
  view_model_factory_register("temp", temperature_view_model_create);
  view_model_factory_register("humi", humidity_view_model_create);
  view_model_t* vm = view_model_factory_create_model("temp+humi", &req);
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(vm);
  ASSERT_EQ(compositor->view_models.size, 2);

  ASSERT_EQ(view_model_get_prop(vm, "temp", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 123);
  ASSERT_EQ(view_model_set_prop(vm, "temp", &v), RET_OK);

  ASSERT_EQ(view_model_get_prop(vm, "humi", &v), RET_OK);
  ASSERT_EQ(view_model_set_prop(vm, "humi", &v), RET_OK);

  ASSERT_EQ(view_model_get_prop(vm, "not_exist", &v), RET_NOT_FOUND);
  ASSERT_EQ(view_model_set_prop(vm, "not_exist", &v), RET_NOT_FOUND);

  ASSERT_EQ(view_model_can_exec(vm, "apply_humi", NULL), TRUE);
  ASSERT_EQ(view_model_can_exec(vm, "apply_temp", NULL), TRUE);

  ASSERT_EQ(view_model_exec(vm, "apply_humi", NULL), RET_OK);
  ASSERT_EQ(view_model_exec(vm, "apply_temp", NULL), RET_OK);

  ASSERT_EQ(view_model_can_exec(vm, "not_exist", NULL), FALSE);
  ASSERT_NE(view_model_exec(vm, "not_exist", NULL), RET_OK);

  OBJECT_UNREF(vm);
  view_model_factory_deinit();
}
