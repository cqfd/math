#include <stan/math/rev/mat.hpp>
#include <gtest/gtest.h>
#include <stan/math/prim/scal/fun/value_of.hpp>
#include <vector>

using Eigen::Dynamic;
using Eigen::Matrix;
using stan::math::var;

//  We check that the values of the new regression match those of one built
//  from existing primitives.
TEST(ProbDistributionsNegBinomial2LogGLM,
     glm_matches_neg_binomial_2_log_doubles) {
  Matrix<int, Dynamic, 1> y(3, 1);
  y << 1, 0, 1;
  Matrix<double, Dynamic, Dynamic> x(3, 2);
  x << -12, 46, -42, 24, 25, 27;
  Matrix<double, Dynamic, 1> beta(2, 1);
  beta << 0.3, 2;
  double alpha = 0.3;
  Matrix<double, Dynamic, 1> alphavec = alpha * Matrix<double, 3, 1>::Ones();
  Matrix<double, Dynamic, 1> theta(3, 1);
  theta = x * beta + alphavec;
  double phi = 2;
  EXPECT_FLOAT_EQ(
      (stan::math::neg_binomial_2_log_lpmf(y, theta, phi)),
      (stan::math::neg_binomial_2_log_glm_lpmf(y, x, alpha, beta, phi)));
  EXPECT_FLOAT_EQ(
      (stan::math::neg_binomial_2_log_lpmf<true>(y, theta, phi)),
      (stan::math::neg_binomial_2_log_glm_lpmf<true>(y, x, alpha, beta, phi)));
}
//  We check that the values of the new regression match those of one built
//  from existing primitives.
TEST(ProbDistributionsNegBinomial2LogGLM,
     glm_matches_neg_binomial_2_log_doubles_rand) {
  for (size_t ii = 0; ii < 42; ii++) {
    Matrix<int, Dynamic, 1> y(3, 1);
    for (size_t i = 0; i < 3; i++) {
      y[i] = Matrix<uint, 1, 1>::Random(1, 1)[0] % 200;
    }
    Matrix<double, Dynamic, Dynamic> x
        = Matrix<double, Dynamic, Dynamic>::Random(3, 2);
    Matrix<double, Dynamic, 1> beta
        = Matrix<double, Dynamic, Dynamic>::Random(2, 1);
    Matrix<double, 1, 1> alphamat = Matrix<double, 1, 1>::Random(1, 1);
    double alpha = alphamat[0];
    Matrix<double, Dynamic, 1> alphavec = alpha * Matrix<double, 3, 1>::Ones();
    Matrix<double, Dynamic, 1> theta(3, 1);
    theta = x * beta + alphavec;
    double phi = Matrix<double, Dynamic, 1>::Random(1, 1)[0] + 1;
    EXPECT_FLOAT_EQ(
        (stan::math::neg_binomial_2_log_lpmf(y, theta, phi)),
        (stan::math::neg_binomial_2_log_glm_lpmf(y, x, alpha, beta, phi)));
    EXPECT_FLOAT_EQ((stan::math::neg_binomial_2_log_lpmf<true>(y, theta, phi)),
                    (stan::math::neg_binomial_2_log_glm_lpmf<true>(y, x, alpha,
                                                                   beta, phi)));
  }
}
//  We check that the gradients of the new regression match those of one built
//  from existing primitives.
TEST(ProbDistributionsNegBinomial2LogGLM, glm_matches_neg_binomial_2_log_vars) {
  Matrix<int, Dynamic, 1> y(3, 1);
  y << 1, 0, 1;
  Matrix<var, Dynamic, Dynamic> x(3, 2);
  x << -12, 46, -42, 24, 25, 27;
  Matrix<var, Dynamic, 1> beta(2, 1);
  beta << 0.3, 2;
  var alpha = 0.3;
  Matrix<var, Dynamic, 1> alphavec = alpha * Matrix<double, 3, 1>::Ones();
  Matrix<var, Dynamic, 1> theta(3, 1);
  theta = x * beta + alphavec;
  var phi = 2;
  var lp = stan::math::neg_binomial_2_log_lpmf(y, theta, phi);
  lp.grad();

  double lp_val = lp.val();
  double alpha_adj = alpha.adj();
  Matrix<double, Dynamic, Dynamic> x_adj(3, 2);
  Matrix<double, Dynamic, 1> beta_adj(2, 1);
  double phi_adj = phi.adj();
  for (size_t i = 0; i < 2; i++) {
    beta_adj[i] = beta[i].adj();
    for (size_t j = 0; j < 3; j++) {
      x_adj(j, i) = x(j, i).adj();
    }
  }

  stan::math::recover_memory();

  Matrix<int, Dynamic, 1> y2(3, 1);
  y2 << 1, 0, 1;
  Matrix<var, Dynamic, Dynamic> x2(3, 2);
  x2 << -12, 46, -42, 24, 25, 27;
  Matrix<var, Dynamic, 1> beta2(2, 1);
  beta2 << 0.3, 2;
  var alpha2 = 0.3;
  var phi2 = 2;
  var lp2
      = stan::math::neg_binomial_2_log_glm_lpmf(y2, x2, alpha2, beta2, phi2);
  lp2.grad();
  EXPECT_FLOAT_EQ(lp_val, lp2.val());
  for (size_t i = 0; i < 2; i++) {
    EXPECT_FLOAT_EQ(beta_adj[i], beta2[i].adj());
  }
  EXPECT_FLOAT_EQ(alpha_adj, alpha2.adj());
  EXPECT_FLOAT_EQ(phi_adj, phi2.adj());
  for (size_t j = 0; j < 3; j++) {
    for (size_t i = 0; i < 2; i++) {
      EXPECT_FLOAT_EQ(x_adj(j, i), x2(j, i).adj());
    }
  }
}

//  We check that the gradients of the new regression match those of one built
//  from existing primitives.
TEST(ProbDistributionsNegBinomial2LogGLM,
     glm_matches_neg_binomial_2_log_vars_rand) {
  for (size_t ii = 0; ii < 200; ii++) {
    Matrix<int, Dynamic, 1> y(3, 1);
    for (size_t i = 0; i < 3; i++) {
      y[i] = Matrix<uint, 1, 1>::Random(1, 1)[0] % 200;
    }
    Matrix<double, Dynamic, Dynamic> xreal
        = Matrix<double, Dynamic, Dynamic>::Random(3, 2);
    Matrix<double, Dynamic, 1> betareal
        = Matrix<double, Dynamic, Dynamic>::Random(2, 1);
    Matrix<double, 1, 1> alphareal = Matrix<double, 1, 1>::Random(1, 1);
    double phireal = Matrix<double, Dynamic, 1>::Random(1, 1)[0] + 1;
    Matrix<var, Dynamic, 1> beta = betareal;
    Matrix<var, Dynamic, 1> theta(3, 1);
    Matrix<var, Dynamic, Dynamic> x = xreal;
    var alpha = alphareal[0];
    Matrix<var, Dynamic, 1> alphavec = Matrix<double, 3, 1>::Ones() * alpha;
    var phi = phireal;
    theta = (x * beta) + alphavec;
    var lp = stan::math::neg_binomial_2_log_lpmf(y, theta, phi);
    lp.grad();

    double lp_val = lp.val();
    double alpha_adj = alpha.adj();
    Matrix<double, Dynamic, Dynamic> x_adj(3, 2);
    Matrix<double, Dynamic, 1> beta_adj(2, 1);
    for (size_t i = 0; i < 2; i++) {
      beta_adj[i] = beta[i].adj();
      for (size_t j = 0; j < 3; j++) {
        x_adj(j, i) = x(j, i).adj();
      }
    }
    double phi_adj = phi.adj();

    stan::math::recover_memory();
    Matrix<var, Dynamic, 1> beta2 = betareal;
    Matrix<var, Dynamic, Dynamic> x2 = xreal;
    var alpha2 = alphareal[0];
    var phi2 = phireal;
    var lp2
        = stan::math::neg_binomial_2_log_glm_lpmf(y, x2, alpha2, beta2, phi2);
    lp2.grad();
    EXPECT_FLOAT_EQ(lp_val, lp2.val());
    for (size_t i = 0; i < 2; i++) {
      EXPECT_FLOAT_EQ(beta_adj[i], beta2[i].adj());
    }
    EXPECT_FLOAT_EQ(alpha_adj, alpha2.adj());
    EXPECT_FLOAT_EQ(phi_adj, phi2.adj());
    for (size_t j = 0; j < 3; j++) {
      for (size_t i = 0; i < 2; i++) {
        EXPECT_FLOAT_EQ(x_adj(j, i), x2(j, i).adj());
      }
    }
  }
}

//  We check that the gradients of the new regression match those of one built
//  from existing primitives, in case beta is a scalar.
TEST(ProbDistributionsNegBinomial2LogGLM,
     glm_matches_neg_binomial_2_log_vars_rand_scal_beta) {
  for (size_t ii = 0; ii < 42; ii++) {
    Matrix<int, Dynamic, 1> y(3, 1);
    for (size_t i = 0; i < 3; i++) {
      y[i] = Matrix<uint, 1, 1>::Random(1, 1)[0] % 200;
    }
    Matrix<double, Dynamic, Dynamic> xreal
        = Matrix<double, Dynamic, Dynamic>::Random(3, 1);
    double betareal = Matrix<double, Dynamic, Dynamic>::Random(1, 1)(0, 0);
    Matrix<double, 1, 1> alphareal = Matrix<double, 1, 1>::Random(1, 1);
    double phireal = Matrix<double, Dynamic, 1>::Random(1, 1)[0] + 1;
    var beta = betareal;
    Matrix<var, Dynamic, 1> theta(3, 1);
    Matrix<var, Dynamic, Dynamic> x = xreal;
    var alpha = alphareal[0];
    Matrix<var, Dynamic, 1> alphavec = Matrix<double, 3, 1>::Ones() * alpha;
    var phi = phireal;
    theta = (x * beta) + alphavec;
    var lp = stan::math::neg_binomial_2_log_lpmf(y, theta, phi);
    lp.grad();

    double lp_val = lp.val();
    double alpha_adj = alpha.adj();
    Matrix<double, Dynamic, Dynamic> x_adj(3, 1);
    double beta_adj = beta.adj();
    for (size_t j = 0; j < 3; j++) {
      x_adj(j, 0) = x(j, 0).adj();
    }
    double phi_adj = phi.adj();

    stan::math::recover_memory();

    var beta2 = betareal;
    Matrix<var, Dynamic, Dynamic> x2 = xreal;
    var alpha2 = alphareal[0];
    var phi2 = phireal;
    var lp2
        = stan::math::neg_binomial_2_log_glm_lpmf(y, x2, alpha2, beta2, phi2);
    lp2.grad();
    EXPECT_FLOAT_EQ(lp_val, lp2.val());
    EXPECT_FLOAT_EQ(beta_adj, beta2.adj());
    EXPECT_FLOAT_EQ(alpha_adj, alpha2.adj());
    EXPECT_FLOAT_EQ(phi_adj, phi2.adj());
    for (size_t j = 0; j < 3; j++) {
      EXPECT_FLOAT_EQ(x_adj(j, 0), x2(j, 0).adj());
    }
  }
}

//  We check that the gradients of the new regression match those of one built
//  from existing primitives.
TEST(ProbDistributionsNegBinomial2LogGLM,
     glm_matches_neg_binomial_2_log_varying_intercept) {
  for (size_t ii = 0; ii < 200; ii++) {
    Matrix<int, Dynamic, 1> y(3, 1);
    for (size_t i = 0; i < 3; i++) {
      y[i] = Matrix<uint, 1, 1>::Random(1, 1)[0] % 200;
    }
    Matrix<double, Dynamic, Dynamic> xreal
        = Matrix<double, Dynamic, Dynamic>::Random(3, 2);
    Matrix<double, Dynamic, 1> betareal
        = Matrix<double, Dynamic, Dynamic>::Random(2, 1);
    Matrix<double, Dynamic, 1> alphareal
        = Matrix<double, Dynamic, Dynamic>::Random(3, 1);
    Matrix<double, Dynamic, 1> phireal
        = Matrix<double, Dynamic, Dynamic>::Random(3, 1)
          + Matrix<double, Dynamic, 1>::Ones(3, 1);
    Matrix<var, Dynamic, 1> beta = betareal;
    Matrix<var, Dynamic, 1> theta(3, 1);
    Matrix<var, Dynamic, Dynamic> x = xreal;
    Matrix<var, Dynamic, 1> alpha = alphareal;
    Matrix<var, Dynamic, 1> phi = phireal;
    theta = (x * beta) + alpha;
    var lp = stan::math::neg_binomial_2_log_lpmf(y, theta, phi);
    lp.grad();

    double lp_val = lp.val();
    Matrix<double, Dynamic, 1> alpha_adj(3, 1);
    Matrix<double, Dynamic, Dynamic> x_adj(3, 2);
    Matrix<double, Dynamic, 1> beta_adj(2, 1);
    Matrix<double, Dynamic, 1> phi_adj(3, 1);
    for (size_t i = 0; i < 2; i++) {
      beta_adj[i] = beta[i].adj();
      for (size_t j = 0; j < 3; j++) {
        x_adj(j, i) = x(j, i).adj();
      }
    }
    for (size_t j = 0; j < 3; j++) {
      phi_adj[j] = phi[j].adj();
      alpha_adj[j] = alpha[j].adj();
    }

    stan::math::recover_memory();
    Matrix<var, Dynamic, 1> beta2 = betareal;
    Matrix<var, Dynamic, Dynamic> x2 = xreal;
    Matrix<var, Dynamic, 1> alpha2 = alphareal;
    Matrix<var, Dynamic, 1> phi2 = phireal;
    var lp2
        = stan::math::neg_binomial_2_log_glm_lpmf(y, x2, alpha2, beta2, phi2);
    lp2.grad();
    EXPECT_FLOAT_EQ(lp_val, lp2.val());
    for (size_t i = 0; i < 2; i++) {
      EXPECT_FLOAT_EQ(beta_adj[i], beta2[i].adj());
    }
    for (size_t j = 0; j < 3; j++) {
      EXPECT_FLOAT_EQ(phi_adj[j], phi2[j].adj());
      EXPECT_FLOAT_EQ(alpha_adj[j], alpha2[j].adj());
      for (size_t i = 0; i < 2; i++) {
        EXPECT_FLOAT_EQ(x_adj(j, i), x2(j, i).adj());
      }
    }
  }
}

//  We check that we can instantiate all different interface types.
TEST(ProbDistributionsNegBinomial2LogGLM,
     glm_matches_neg_binomial_2_log_interface_types) {
  double value = 0;
  double value2 = 0;

  int i = 1;
  std::vector<double> vi = {{1, 0}};
  double d = 1.0;
  std::vector<double> vd = {{1.0, 2.0}};
  Eigen::VectorXd ev(2);
  Eigen::RowVectorXd rv(2);
  Eigen::MatrixXd m1(1, 1);
  m1 << 1.0;
  Eigen::MatrixXd m(2, 2);
  ev << 1.0, 2.0;
  rv << 1.0, 2.0;
  m << 1.0, 2.0, 3.0, 4.0;

  value += stan::math::neg_binomial_2_log_glm_lpmf(i, m1, d, d, d);
  value += stan::math::neg_binomial_2_log_glm_lpmf(vi, m, vd, vd, vd);
  value += stan::math::neg_binomial_2_log_glm_lpmf(vi, m, ev, ev, ev);
  value += stan::math::neg_binomial_2_log_glm_lpmf(vi, m, rv, rv, rv);

  var v = 1.0;
  std::vector<var> vv = {{1.0, 2.0}};
  Eigen::Matrix<var, -1, 1> evv(2);
  Eigen::Matrix<var, 1, -1> rvv(2);
  Eigen::Matrix<var, -1, -1> m1v(1, 1);
  m1v << 1.0;
  Eigen::Matrix<var, -1, -1> mv(2, 2);
  evv << 1.0, 2.0;
  rvv << 1.0, 2.0;
  mv << 1.0, 2.0, 3.0, 4.0;

  value2 += stan::math::neg_binomial_2_log_glm_lpmf(i, m1v, v, v, v).val();
  value2 += stan::math::neg_binomial_2_log_glm_lpmf(vi, mv, vv, vv, vv).val();
  value2
      += stan::math::neg_binomial_2_log_glm_lpmf(vi, mv, evv, evv, evv).val();
  value2
      += stan::math::neg_binomial_2_log_glm_lpmf(vi, mv, rvv, rvv, rvv).val();

  EXPECT_FLOAT_EQ(value, value2);
}

//  We check that the right errors are thrown.
TEST(ProbDistributionsNegBinomial2LogGLM,
     glm_matches_neg_binomial_2_log_error_checking) {
  int N = 3;
  int M = 2;
  int W = 4;

  Eigen::Matrix<int, -1, 1> y(N, 1);
  for (int n = 0; n < N; n++) {
    y[n] = Eigen::Matrix<uint, -1, 1>::Random(1, 1)[0] % 200;
  }
  Eigen::Matrix<int, -1, 1> yw1(W, 1);
  for (int n = 0; n < W; n++) {
    yw1[n] = Eigen::Matrix<uint, -1, 1>::Random(1, 1)[0] % 200;
  }
  Eigen::Matrix<int, -1, 1> yw2(N, 1);
  for (int n = 0; n < N; n++) {
    yw2[n] = -(Eigen::Matrix<uint, -1, 1>::Random(1, 1)[0] % 200);
  }
  Eigen::Matrix<double, -1, -1> x = Eigen::Matrix<double, -1, -1>::Random(N, M);
  Eigen::Matrix<double, -1, -1> xw1
      = Eigen::Matrix<double, -1, -1>::Random(W, M);
  Eigen::Matrix<double, -1, -1> xw2
      = Eigen::Matrix<double, -1, -1>::Random(N, W);
  Eigen::Matrix<double, -1, -1> xw3
      = Eigen::Matrix<double, -1, -1>::Random(N, M) * NAN;
  Eigen::Matrix<double, -1, 1> alpha
      = Eigen::Matrix<double, -1, 1>::Random(N, 1);
  Eigen::Matrix<double, -1, 1> alphaw1
      = Eigen::Matrix<double, -1, 1>::Random(W, 1);
  Eigen::Matrix<double, -1, 1> alphaw2
      = Eigen::Matrix<double, -1, 1>::Random(N, 1) * NAN;
  Eigen::Matrix<double, -1, 1> beta
      = Eigen::Matrix<double, -1, 1>::Random(M, 1);
  Eigen::Matrix<double, -1, 1> betaw1
      = Eigen::Matrix<double, -1, 1>::Random(W, 1);
  Eigen::Matrix<double, -1, 1> betaw2
      = Eigen::Matrix<double, -1, 1>::Random(M, 1) * NAN;
  Eigen::Matrix<double, -1, 1> sigma
      = Eigen::Matrix<double, -1, 1>::Random(N, 1)
        + Eigen::Matrix<double, -1, 1>::Ones(N, 1);
  Eigen::Matrix<double, -1, 1> sigmaw1
      = Eigen::Matrix<double, -1, 1>::Random(W, 1)
        + Eigen::Matrix<double, -1, 1>::Ones(W, 1);
  Eigen::Matrix<double, -1, 1> sigmaw2
      = Eigen::Matrix<double, -1, 1>::Random(N, 1)
        - Eigen::Matrix<double, -1, 1>::Ones(N, 1);
  Eigen::Matrix<double, -1, 1> sigmaw3
      = (Eigen::Matrix<double, -1, 1>::Random(N, 1)
         + Eigen::Matrix<double, -1, 1>::Ones(N, 1))
        * NAN;

  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(yw1, x, alpha, beta, sigma),
      std::invalid_argument);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(yw2, x, alpha, beta, sigma),
      std::domain_error);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(y, xw1, alpha, beta, sigma),
      std::invalid_argument);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(y, xw2, alpha, beta, sigma),
      std::invalid_argument);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(y, xw3, alpha, beta, sigma),
      std::domain_error);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(y, x, alphaw1, beta, sigma),
      std::invalid_argument);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(y, x, alphaw2, beta, sigma),
      std::domain_error);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(y, x, alpha, betaw1, sigma),
      std::invalid_argument);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(y, x, alpha, betaw2, sigma),
      std::domain_error);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(y, x, alpha, beta, sigmaw1),
      std::invalid_argument);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(y, x, alpha, beta, sigmaw2),
      std::domain_error);
  EXPECT_THROW(
      stan::math::neg_binomial_2_log_glm_lpmf(y, x, alpha, beta, sigmaw3),
      std::domain_error);
}
