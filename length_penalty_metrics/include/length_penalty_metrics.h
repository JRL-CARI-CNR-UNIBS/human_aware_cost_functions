#pragma once
/*
Copyright (c) 2023, Cesare Tonola University of Brescia c.tonola001@unibs.it
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <graph_core/metrics.h>
#include <graph_core/graph/node.h>
#include <ssm15066_estimators/ssm15066_estimator.h>

namespace pathplan
{
class LengthPenaltyMetrics;
typedef std::shared_ptr<LengthPenaltyMetrics> LengthPenaltyMetricsPtr;

/**
 * @brief The LengthPenaltyMetrics class computes the Euclidean distance between two nodes and penalizes
 * it based on an estimation on how much the robot would be slown down by the safety velocity scaling
 * unit, due to being close to an obstacle (e.g., human being) while crossing that connection.
 * So, it computes an approximation of the average scaling factor (lambda) of the connection and then the cost is
 *
 *                            c(q1,q2) = ||q2-q1||*lambda,
 *
 * where lambda = (1+(t_safety - t_nom)/t_nom) ~= mean(max(v_r(i)/v_safety(i),1)) (>= 1.0) for each qi belonging to (q1,q2)
*/

class LengthPenaltyMetrics: public Metrics
{
protected:
  ssm15066_estimator::SSM15066EstimatorPtr ssm15066_estimator_;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  static constexpr double lambda_penalty_ = 1.0e12;

  LengthPenaltyMetrics(const ssm15066_estimator::SSM15066EstimatorPtr& ssm15066_estimator);

  ssm15066_estimator::SSM15066EstimatorPtr getSSM()
  {
    return ssm15066_estimator_;
  }

  virtual double cost(const NodePtr& node1,
                      const NodePtr& node2);

  virtual double cost(const Eigen::VectorXd& configuration1,
                      const Eigen::VectorXd& configuration2);


  virtual double utopia(const NodePtr& node1,
                        const NodePtr& node2);

  virtual double utopia(const Eigen::VectorXd& configuration1,
                        const Eigen::VectorXd& configuration2);

  virtual MetricsPtr clone();

};
}
