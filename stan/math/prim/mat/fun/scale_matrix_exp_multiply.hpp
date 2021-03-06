#ifndef STAN_MATH_PRIM_MAT_FUN_SCALE_MATRIX_EXP_MULTIPLY_HPP
#define STAN_MATH_PRIM_MAT_FUN_SCALE_MATRIX_EXP_MULTIPLY_HPP

#include <stan/math/prim/mat.hpp>
#include <stan/math/prim/mat/fun/matrix_exp_action_handler.hpp>

namespace stan {
namespace math {

/**
 * Return product of exp(At) and B, where A is a NxN double matrix,
 * B is a NxCb double matrix, and t is a double
 * @tparam Cb Columns matrix B
 * @param[in] A Matrix
 * @param[in] B Matrix
 * @param[in] t double
 * @return exponential of At multiplies B
 */
template <int Cb>
inline Eigen::Matrix<double, -1, Cb> scale_matrix_exp_multiply(
    const double& t, const Eigen::MatrixXd& A,
    const Eigen::Matrix<double, -1, Cb>& B) {
  check_nonzero_size("scale_matrix_exp_multiply", "input matrix", A);
  check_nonzero_size("scale_matrix_exp_multiply", "input matrix", B);
  check_multiplicable("scale_matrix_exp_multiply", "A", A, "B", B);
  check_square("scale_matrix_exp_multiply", "input matrix", A);
  return matrix_exp_action_handler().action(A, B, t);
}

}  // namespace math
}  // namespace stan
#endif
