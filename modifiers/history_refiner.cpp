/*********************                                                  */
/*! \file history_refiner.h
** \verbatim
** Top contributors (to current version):
**   Makai Mann
** This file is part of the pono project.
** Copyright (c) 2019 by the authors listed in the file AUTHORS
** in the top-level source directory) and their institutional affiliations.
** All rights reserved.  See the file LICENSE in the top-level source
** directory for licensing information.\endverbatim
**
** \brief Class for adding history variables to a system.
**
**/

#include "modifiers/history_refiner.h"

using namespace smt;
using namespace std;

namespace pono {

HistoryRefiner::HistoryRefiner(TransitionSystem & ts)
    : ts_(ts), solver_(ts_.solver())
{
}

Term HistoryRefiner::get_hist(const Term & target, size_t delay)
{
  if (!delay) {
    // zero dealy is itself
    return target;
  }

  Sort sort = target->get_sort();

  // create new history variables if needed
  string name;
  Term var;
  size_t num_existing_hist_vars = hist_vars_[target].size();
  while (num_existing_hist_vars < delay) {
    num_existing_hist_vars++;
    name = "hist_" + target->to_string() + to_string(num_existing_hist_vars);
    var = ts_.make_statevar(name, sort);

    if (num_existing_hist_vars == 1) {
      // hist_x_1' = x
      ts_.assign_next(var, target);
    } else {
      // hist_x_n' = hist_x_{n-1}
      ts_.assign_next(var, hist_vars_[target].back());
    }
  }

  // get the desired variable
  // Note: needs to work even if loop above didn't run
  var = hist_vars_.at(target).at(delay - 1);
  return var;
}

}  // namespace pono
