#ifdef STAN_OPENCL
#include <stan/math/prim/mat.hpp>
#include <stan/math/gpu/matrix_gpu.hpp>
#include <gtest/gtest.h>
#include <algorithm>

TEST(MathMatrixGPU, triangular_transpose_m_exception_pass) {
  stan::math::matrix_gpu m1(1, 1);
  stan::math::matrix_gpu m0;
  stan::math::matrix_gpu m2(5, 3);
  stan::math::matrix_gpu m3(3, 4);

  EXPECT_NO_THROW(m0.triangular_transpose<stan::math::gpu::LowerToUpper>());
  EXPECT_NO_THROW(m0.triangular_transpose<stan::math::gpu::UpperToLower>());
  EXPECT_NO_THROW(m1.triangular_transpose<stan::math::gpu::LowerToUpper>());
  EXPECT_NO_THROW(m1.triangular_transpose<stan::math::gpu::UpperToLower>());
  EXPECT_THROW(m2.triangular_transpose<stan::math::gpu::LowerToUpper>(),
               std::invalid_argument);
  EXPECT_THROW(m2.triangular_transpose<stan::math::gpu::UpperToLower>(),
               std::invalid_argument);
  EXPECT_THROW(m3.triangular_transpose<stan::math::gpu::LowerToUpper>(),
               std::invalid_argument);
  EXPECT_THROW(m3.triangular_transpose<stan::math::gpu::UpperToLower>(),
               std::invalid_argument);
}

TEST(MathMatrixGPU, triangular_transpose_m_pass) {
  stan::math::matrix_d m0(2, 2);
  stan::math::matrix_d m0_dst(2, 2);
  m0 << 1, 2, 3, 4;

  stan::math::matrix_gpu m00(m0);
  stan::math::matrix_gpu m11(m0);

  EXPECT_NO_THROW(m00.triangular_transpose<stan::math::gpu::LowerToUpper>());
  EXPECT_NO_THROW(stan::math::copy(m0_dst, m00));
  EXPECT_EQ(1, m0_dst(0, 0));
  EXPECT_EQ(3, m0_dst(0, 1));
  EXPECT_EQ(3, m0_dst(1, 0));
  EXPECT_EQ(4, m0_dst(1, 1));

  EXPECT_NO_THROW(m11.triangular_transpose<stan::math::gpu::UpperToLower>());
  EXPECT_NO_THROW(stan::math::copy(m0_dst, m11));
  EXPECT_EQ(1, m0_dst(0, 0));
  EXPECT_EQ(2, m0_dst(0, 1));
  EXPECT_EQ(2, m0_dst(1, 0));
  EXPECT_EQ(4, m0_dst(1, 1));
}

#endif
