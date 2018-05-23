/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include <gtest/gtest.h>
#include "tensorflow/contrib/lite/interpreter.h"
#include "tensorflow/contrib/lite/kernels/register.h"
#include "tensorflow/contrib/lite/kernels/test_util.h"
#include "tensorflow/contrib/lite/model.h"

namespace tflite {
namespace {

using ::testing::ElementsAreArray;

class GreaterOpModel : public SingleOpModel {
 public:
  GreaterOpModel(std::initializer_list<int> input1_shape,
                 std::initializer_list<int> input2_shape,
                 TensorType input_type) {
    input1_ = AddInput(input_type);
    input2_ = AddInput(input_type);
    output_ = AddOutput(TensorType_BOOL);
    SetBuiltinOp(BuiltinOperator_GREATER, BuiltinOptions_GreaterOptions,
                 CreateGreaterOptions(builder_).Union());
    BuildInterpreter({input1_shape, input2_shape});
  }

  int input1() { return input1_; }
  int input2() { return input2_; }

  std::vector<bool> GetOutput() { return ExtractVector<bool>(output_); }
  std::vector<int> GetOutputShape() { return GetTensorShape(output_); }

 private:
  int input1_;
  int input2_;
  int output_;
};

TEST(ComparisonsTest, GreaterFloat) {
  GreaterOpModel model({1, 1, 1, 4}, {1, 1, 1, 4}, TensorType_FLOAT32);
  model.PopulateTensor<float>(model.input1(), {0.1, 0.9, 0.7, 0.3});
  model.PopulateTensor<float>(model.input2(), {0.1, 0.2, 0.6, 0.5});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({false, true, true, false}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, GreaterInt) {
  GreaterOpModel model({1, 1, 1, 4}, {1, 1, 1, 4}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3});
  model.PopulateTensor<int>(model.input2(), {1, 2, 7, 5});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({false, true, false, false}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, GreaterBroadcast) {
  GreaterOpModel model({1, 1, 1, 4}, {1, 1, 1, 1}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3});
  model.PopulateTensor<int>(model.input2(), {7});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({false, true, false, false}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, GreaterBroadcastTwoD) {
  GreaterOpModel model({1, 1, 2, 4}, {1, 1, 1, 4}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3, 2, 4, 2, 8});
  model.PopulateTensor<int>(model.input2(), {7, 1, 2, 4});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({false, true, true, false,
                                                   false, true, false, true}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 2, 4}));
}

class GreaterEqualOpModel : public SingleOpModel {
 public:
  GreaterEqualOpModel(std::initializer_list<int> input1_shape,
                      std::initializer_list<int> input2_shape,
                      TensorType input_type) {
    input1_ = AddInput(input_type);
    input2_ = AddInput(input_type);
    output_ = AddOutput(TensorType_BOOL);
    SetBuiltinOp(BuiltinOperator_GREATER_EQUAL,
                 BuiltinOptions_GreaterEqualOptions,
                 CreateGreaterEqualOptions(builder_).Union());
    BuildInterpreter({input1_shape, input2_shape});
  }

  int input1() { return input1_; }
  int input2() { return input2_; }

  std::vector<bool> GetOutput() { return ExtractVector<bool>(output_); }
  std::vector<int> GetOutputShape() { return GetTensorShape(output_); }

 private:
  int input1_;
  int input2_;
  int output_;
};

TEST(ComparisonsTest, GreaterEqualFloat) {
  GreaterEqualOpModel model({1, 1, 1, 4}, {1, 1, 1, 4}, TensorType_FLOAT32);
  model.PopulateTensor<float>(model.input1(), {0.1, 0.9, 0.7, 0.3});
  model.PopulateTensor<float>(model.input2(), {0.1, 0.2, 0.6, 0.5});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({true, true, true, false}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, GreaterEqualInt) {
  GreaterEqualOpModel model({1, 1, 1, 4}, {1, 1, 1, 4}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3});
  model.PopulateTensor<int>(model.input2(), {1, 2, 7, 5});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({false, true, true, false}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, GreaterEqualBroadcast) {
  GreaterEqualOpModel model({1, 1, 1, 4}, {1, 1, 1, 1}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3});
  model.PopulateTensor<int>(model.input2(), {7});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({false, true, true, false}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, GreaterEqualBroadcastTwoD) {
  GreaterEqualOpModel model({1, 1, 2, 4}, {1, 1, 1, 4}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3, 2, 4, 2, 8});
  model.PopulateTensor<int>(model.input2(), {7, 1, 2, 4});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({false, true, true, false,
                                                   false, true, true, true}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 2, 4}));
}

class LessOpModel : public SingleOpModel {
 public:
  LessOpModel(std::initializer_list<int> input1_shape,
              std::initializer_list<int> input2_shape, TensorType input_type) {
    input1_ = AddInput(input_type);
    input2_ = AddInput(input_type);
    output_ = AddOutput(TensorType_BOOL);
    SetBuiltinOp(BuiltinOperator_LESS, BuiltinOptions_LessOptions,
                 CreateLessOptions(builder_).Union());
    BuildInterpreter({input1_shape, input2_shape});
  }

  int input1() { return input1_; }
  int input2() { return input2_; }

  std::vector<bool> GetOutput() { return ExtractVector<bool>(output_); }
  std::vector<int> GetOutputShape() { return GetTensorShape(output_); }

 private:
  int input1_;
  int input2_;
  int output_;
};

TEST(ComparisonsTest, LessFloat) {
  LessOpModel model({1, 1, 1, 4}, {1, 1, 1, 4}, TensorType_FLOAT32);
  model.PopulateTensor<float>(model.input1(), {0.1, 0.9, 0.7, 0.3});
  model.PopulateTensor<float>(model.input2(), {0.1, 0.2, 0.6, 0.5});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({false, false, false, true}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, LessInt) {
  LessOpModel model({1, 1, 1, 4}, {1, 1, 1, 4}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3});
  model.PopulateTensor<int>(model.input2(), {1, 2, 6, 5});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({true, false, false, true}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, LessBroadcast) {
  LessOpModel model({1, 1, 1, 4}, {1, 1, 1, 1}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3});
  model.PopulateTensor<int>(model.input2(), {7});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({true, false, false, true}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, LessBroadcastTwoD) {
  LessOpModel model({1, 1, 2, 4}, {1, 1, 1, 4}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3, 2, 4, 6, 8});
  model.PopulateTensor<int>(model.input2(), {7, 1, 2, 4});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({true, false, false, true,
                                                   true, false, false, false}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 2, 4}));
}

class LessEqualOpModel : public SingleOpModel {
 public:
  LessEqualOpModel(std::initializer_list<int> input1_shape,
                   std::initializer_list<int> input2_shape,
                   TensorType input_type) {
    input1_ = AddInput(input_type);
    input2_ = AddInput(input_type);
    output_ = AddOutput(TensorType_BOOL);
    SetBuiltinOp(BuiltinOperator_LESS_EQUAL, BuiltinOptions_LessEqualOptions,
                 CreateLessEqualOptions(builder_).Union());
    BuildInterpreter({input1_shape, input2_shape});
  }

  int input1() { return input1_; }
  int input2() { return input2_; }

  std::vector<bool> GetOutput() { return ExtractVector<bool>(output_); }
  std::vector<int> GetOutputShape() { return GetTensorShape(output_); }

 private:
  int input1_;
  int input2_;
  int output_;
};

TEST(ComparisonsTest, LessEqualFloat) {
  LessEqualOpModel model({1, 1, 1, 4}, {1, 1, 1, 4}, TensorType_FLOAT32);
  model.PopulateTensor<float>(model.input1(), {0.1, 0.9, 0.7, 0.3});
  model.PopulateTensor<float>(model.input2(), {0.1, 0.2, 0.6, 0.5});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({true, false, false, true}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, LessEqualInt) {
  LessEqualOpModel model({1, 1, 1, 4}, {1, 1, 1, 4}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3});
  model.PopulateTensor<int>(model.input2(), {1, 2, 7, 5});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({true, false, true, true}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, LessEqualBroadcast) {
  LessEqualOpModel model({1, 1, 1, 4}, {1, 1, 1, 1}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3});
  model.PopulateTensor<int>(model.input2(), {7});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({true, false, true, true}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 1, 4}));
}

TEST(ComparisonsTest, LessEqualBroadcastTwoD) {
  LessEqualOpModel model({1, 1, 2, 4}, {1, 1, 1, 4}, TensorType_INT32);
  model.PopulateTensor<int>(model.input1(), {-1, 9, 7, 3, 2, 4, 2, 8});
  model.PopulateTensor<int>(model.input2(), {7, 1, 2, 4});
  model.Invoke();

  EXPECT_THAT(model.GetOutput(), ElementsAreArray({true, false, false, true,
                                                   true, false, true, false}));
  EXPECT_THAT(model.GetOutputShape(), ElementsAreArray({1, 1, 2, 4}));
}

}  // namespace
}  // namespace tflite

int main(int argc, char** argv) {
  ::tflite::LogToStderr();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
