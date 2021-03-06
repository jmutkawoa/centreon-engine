/*
** Copyright 2011-2015 Merethis
**
** This file is part of Centreon Engine.
**
** Centreon Engine is free software: you can redistribute it and/or
** modify it under the terms of the GNU General Public License version 2
** as published by the Free Software Foundation.
**
** Centreon Engine is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Centreon Engine. If not, see
** <http://www.gnu.org/licenses/>.
*/

#include "com/centreon/engine/broker.hh"
#include "com/centreon/engine/configuration/applier/state.hh"
#include "com/centreon/engine/deleter/service.hh"
#include "com/centreon/engine/error.hh"
#include "com/centreon/engine/globals.hh"
#include "com/centreon/engine/logging/logger.hh"
#include "com/centreon/engine/objects/commandsmember.hh"
#include "com/centreon/engine/objects/customvariablesmember.hh"
#include "com/centreon/engine/objects/service.hh"
#include "com/centreon/engine/objects/tool.hh"
#include "com/centreon/engine/shared.hh"
#include "com/centreon/engine/string.hh"
#include "com/centreon/shared_ptr.hh"

using namespace com::centreon;
using namespace com::centreon::engine;
using namespace com::centreon::engine::configuration::applier;
using namespace com::centreon::engine::logging;
using namespace com::centreon::engine::string;

/**
 *  Equal operator.
 *
 *  @param[in] obj1 The first object to compare.
 *  @param[in] obj2 The second object to compare.
 *
 *  @return True if is the same object, otherwise false.
 */
bool operator==(
       service const& obj1,
       service const& obj2) throw () {
  return (is_equal(obj1.host_name, obj2.host_name)
          && is_equal(obj1.description, obj2.description)
          && is_equal(obj1.service_check_command, obj2.service_check_command)
          && is_equal(obj1.event_handler, obj2.event_handler)
          && obj1.initial_state == obj2.initial_state
          && obj1.check_interval == obj2.check_interval
          && obj1.retry_interval == obj2.retry_interval
          && obj1.max_attempts == obj2.max_attempts
          && obj1.check_timeout == obj2.check_timeout
          && obj1.is_volatile == obj2.is_volatile
          && is_equal(obj1.check_period, obj2.check_period)
          && obj1.flap_detection_enabled == obj2.flap_detection_enabled
          && obj1.low_flap_threshold == obj2.low_flap_threshold
          && obj1.high_flap_threshold == obj2.high_flap_threshold
          && obj1.flap_detection_on_ok == obj2.flap_detection_on_ok
          && obj1.flap_detection_on_warning == obj2.flap_detection_on_warning
          && obj1.flap_detection_on_unknown == obj2.flap_detection_on_unknown
          && obj1.flap_detection_on_critical == obj2.flap_detection_on_critical
          && obj1.check_freshness == obj2.check_freshness
          && obj1.freshness_threshold == obj2.freshness_threshold
          && obj1.event_handler_enabled == obj2.event_handler_enabled
          && obj1.checks_enabled == obj2.checks_enabled
          && obj1.obsess_over_service == obj2.obsess_over_service
          && is_equal(obj1.custom_variables, obj2.custom_variables)
          && obj1.host_problem_at_last_check == obj2.host_problem_at_last_check
          && obj1.check_type == obj2.check_type
          && obj1.current_state == obj2.current_state
          && obj1.last_state == obj2.last_state
          && obj1.last_hard_state == obj2.last_hard_state
          && is_equal(obj1.plugin_output, obj2.plugin_output)
          && is_equal(obj1.long_plugin_output, obj2.long_plugin_output)
          && is_equal(obj1.perf_data, obj2.perf_data)
          && obj1.state_type == obj2.state_type
          && obj1.next_check == obj2.next_check
          && obj1.should_be_scheduled == obj2.should_be_scheduled
          && obj1.last_check == obj2.last_check
          && obj1.current_attempt == obj2.current_attempt
          && obj1.current_event_id == obj2.current_event_id
          && obj1.last_event_id == obj2.last_event_id
          && obj1.current_problem_id == obj2.current_problem_id
          && obj1.last_problem_id == obj2.last_problem_id
          && obj1.last_state_change == obj2.last_state_change
          && obj1.last_hard_state_change == obj2.last_hard_state_change
          && obj1.last_time_ok == obj2.last_time_ok
          && obj1.last_time_warning == obj2.last_time_warning
          && obj1.last_time_unknown == obj2.last_time_unknown
          && obj1.last_time_critical == obj2.last_time_critical
          && obj1.has_been_checked == obj2.has_been_checked
          && obj1.is_being_freshened == obj2.is_being_freshened
          && obj1.latency == obj2.latency
          && obj1.execution_time == obj2.execution_time
          && obj1.is_executing == obj2.is_executing
          && obj1.check_options == obj2.check_options
          && is_equal(obj1.state_history, obj2.state_history, MAX_STATE_HISTORY_ENTRIES)
          && obj1.state_history_index == obj2.state_history_index
          && obj1.is_flapping == obj2.is_flapping
          && obj1.percent_state_change == obj2.percent_state_change
          && obj1.modified_attributes == obj2.modified_attributes
          && is_equal(obj1.event_handler_args, obj2.event_handler_args)
          && is_equal(obj1.check_command_args, obj2.check_command_args)
          && is_equal(obj1.timezone, obj2.timezone));
}

/**
 *  Not equal operator.
 *
 *  @param[in] obj1 The first object to compare.
 *  @param[in] obj2 The second object to compare.
 *
 *  @return True if is not the same object, otherwise false.
 */
bool operator!=(
       service const& obj1,
       service const& obj2) throw () {
  return (!operator==(obj1, obj2));
}

/**
 *  Dump service content into the stream.
 *
 *  @param[out] os  The output stream.
 *  @param[in]  obj The service to dump.
 *
 *  @return The output stream.
 */
std::ostream& operator<<(std::ostream& os, service const& obj) {
  char const* hst_str(NULL);
  if (obj.host_ptr)
    hst_str = chkstr(obj.host_ptr->name);
  char const* evt_str(NULL);
  if (obj.event_handler_ptr)
    evt_str = chkstr(obj.event_handler_ptr->name);
  char const* cmd_str(NULL);
  if (obj.check_command_ptr)
    cmd_str = chkstr(obj.check_command_ptr->name);
  char const* chk_period_str(NULL);
  if (obj.check_period_ptr)
    chk_period_str = chkstr(obj.check_period_ptr->name);

  os << "service {\n"
    "  host_name:                            " << chkstr(obj.host_name) << "\n"
    "  description:                          " << chkstr(obj.description) << "\n"
    "  service_check_command:                " << chkstr(obj.service_check_command) << "\n"
    "  event_handler:                        " << chkstr(obj.event_handler) << "\n"
    "  initial_state:                        " << obj.initial_state << "\n"
    "  check_interval:                       " << obj.check_interval << "\n"
    "  retry_interval:                       " << obj.retry_interval << "\n"
    "  max_attempts:                         " << obj.max_attempts << "\n"
    "  check_timeout:                        " << obj.check_timeout << "\n"
    "  is_volatile:                          " << obj.is_volatile << "\n"
    "  check_period:                         " << chkstr(obj.check_period) << "\n"
    "  flap_detection_enabled:               " << obj.flap_detection_enabled << "\n"
    "  low_flap_threshold:                   " << obj.low_flap_threshold << "\n"
    "  high_flap_threshold:                  " << obj.high_flap_threshold << "\n"
    "  flap_detection_on_ok:                 " << obj.flap_detection_on_ok << "\n"
    "  flap_detection_on_warning:            " << obj.flap_detection_on_warning << "\n"
    "  flap_detection_on_unknown:            " << obj.flap_detection_on_unknown << "\n"
    "  flap_detection_on_critical:           " << obj.flap_detection_on_critical << "\n"
    "  check_freshness:                      " << obj.check_freshness << "\n"
    "  freshness_threshold:                  " << obj.freshness_threshold << "\n"
    "  event_handler_enabled:                " << obj.event_handler_enabled << "\n"
    "  checks_enabled:                       " << obj.checks_enabled << "\n"
    "  obsess_over_service:                  " << obj.obsess_over_service << "\n"
    "  host_problem_at_last_check:           " << obj.host_problem_at_last_check << "\n"
    "  check_type:                           " << obj.check_type << "\n"
    "  current_state:                        " << obj.current_state << "\n"
    "  last_state:                           " << obj.last_state << "\n"
    "  last_hard_state:                      " << obj.last_hard_state << "\n"
    "  plugin_output:                        " << chkstr(obj.plugin_output) << "\n"
    "  long_plugin_output:                   " << chkstr(obj.long_plugin_output) << "\n"
    "  perf_data:                            " << chkstr(obj.perf_data) << "\n"
    "  state_type:                           " << obj.state_type << "\n"
    "  next_check:                           " << string::ctime(obj.next_check) << "\n"
    "  should_be_scheduled:                  " << obj.should_be_scheduled << "\n"
    "  last_check:                           " << string::ctime(obj.last_check) << "\n"
    "  current_attempt:                      " << obj.current_attempt << "\n"
    "  current_event_id:                     " << obj.current_event_id << "\n"
    "  last_event_id:                        " << obj.last_event_id << "\n"
    "  current_problem_id:                   " << obj.current_problem_id << "\n"
    "  last_problem_id:                      " << obj.last_problem_id << "\n"
    "  last_state_change:                    " << string::ctime(obj.last_state_change) << "\n"
    "  last_hard_state_change:               " << string::ctime(obj.last_hard_state_change) << "\n"
    "  last_time_ok:                         " << string::ctime(obj.last_time_ok) << "\n"
    "  last_time_warning:                    " << string::ctime(obj.last_time_warning) << "\n"
    "  last_time_unknown:                    " << string::ctime(obj.last_time_unknown) << "\n"
    "  last_time_critical:                   " << string::ctime(obj.last_time_critical) << "\n"
    "  has_been_checked:                     " << obj.has_been_checked << "\n"
    "  is_being_freshened:                   " << obj.is_being_freshened << "\n"
    "  latency:                              " << obj.latency << "\n"
    "  execution_time:                       " << obj.execution_time << "\n"
    "  is_executing:                         " << obj.is_executing << "\n"
    "  check_options:                        " << obj.check_options << "\n"
    "  timezone:                             " << chkstr(obj.timezone) << "\n";

  os << "  state_history:                        ";
  for (unsigned int i(0), end(sizeof(obj.state_history) / sizeof(obj.state_history[0]));
       i < end;
       ++i)
    os << obj.state_history[i] << (i + 1 < end ? ", " : "\n");

  os <<
    "  state_history_index:                  " << obj.state_history_index << "\n"
    "  is_flapping:                          " << obj.is_flapping << "\n"
    "  percent_state_change:                 " << obj.percent_state_change << "\n"
    "  modified_attributes:                  " << obj.modified_attributes << "\n"
    "  host_ptr:                             " << chkstr(hst_str) << "\n"
    "  event_handler_ptr:                    " << chkstr(evt_str) << "\n"
    "  event_handler_args:                   " << chkstr(obj.event_handler_args) << "\n"
    "  check_command_ptr:                    " << chkstr(cmd_str) << "\n"
    "  check_command_args:                   " << chkstr(obj.check_command_args) << "\n"
    "  check_period_ptr:                     " << chkstr(chk_period_str) << "\n"
    << (obj.custom_variables ? chkobj(obj.custom_variables) : "")
    << "}\n";
  return (os);
}

/**
 *  Add a new service to the list in memory.
 *
 *  @param[in] host_id                      The host id.
 *  @param[in] host_name                    Name of the host this
 *                                          service is running on.
 *  @param[in] service_id                   The service id.
 *  @param[in] description                  Service description.
 *  @param[in] check_period                 Check timeperiod name.
 *  @param[in] initial_state                Initial service state.
 *  @param[in] max_attempts                 Max check attempts.
 *  @param[in] check_timeout                Check timeout.
 *  @param[in] check_interval               Normal check interval.
 *  @param[in] retry_interval               Retry check interval.
 *  @param[in] is_volatile                  Is this service volatile ?
 *  @param[in] event_handler                Event handler command name.
 *  @param[in] event_handler_enabled        Whether or not event handler
 *                                          is enabled.
 *  @param[in] check_command                Active check command name.
 *  @param[in] checks_enabled               Are active checks enabled ?
 *  @param[in] flap_detection_enabled       Whether or not flap
 *                                          detection is enabled.
 *  @param[in] low_flap_threshold           Low flap threshold.
 *  @param[in] high_flap_threshold          High flap threshold.
 *  @param[in] flap_detection_on_ok         Is flap detection enabled
 *                                          for ok state ?
 *  @param[in] flap_detection_on_warning    Is flap detection enabled
 *                                          for warning state ?
 *  @param[in] flap_detection_on_unknown    Is flap detection enabled
 *                                          for unknown state ?
 *  @param[in] flap_detection_on_critical   Is flap detection enabled
 *                                          for critical state ?
 *  @param[in] check_freshness              Enable freshness check ?
 *  @param[in] freshness_threshold          Freshness threshold.
 *  @param[in] obsess_over_service          Should we obsess over
 *                                          service ?
 *  @param[in] timezone                     Service timezone.
 *
 *  @return New service.
 */
service* add_service(
           unsigned int host_id,
           char const* host_name,
           unsigned int service_id,
           char const* description,
           char const* check_period,
           int initial_state,
           int max_attempts,
           unsigned int check_timeout,
           double check_interval,
           double retry_interval,
           int is_volatile,
           char const* event_handler,
           int event_handler_enabled,
           char const* check_command,
           int checks_enabled,
           int flap_detection_enabled,
           double low_flap_threshold,
           double high_flap_threshold,
           int flap_detection_on_ok,
           int flap_detection_on_warning,
           int flap_detection_on_unknown,
           int flap_detection_on_critical,
           int check_freshness,
           int freshness_threshold,
           int obsess_over_service,
           char const* timezone) {
  // Make sure we have everything we need.
  if (!description || !description[0]) {
    logger(log_config_error, basic)
      << "Error: Service description is not set";
    return (NULL);
  }
  else if (!host_name || !host_name[0]) {
    logger(log_config_error, basic)
      << "Error: Host name of service '"
      << description << "' is not set";
    return (NULL);
  }

  // Check values.
  if ((max_attempts <= 0)
      || (check_interval < 0)
      || (retry_interval < 0)) {
    logger(log_config_error, basic)
      << "Error: Invalid max_attempts, check_interval or retry_interval"
         " value for service '"
      << description << "' on host '" << host_name << "'";
    return (NULL);
  }

  // Check if the service is already exist.
  std::pair<std::string, std::string>
    id(std::make_pair(host_name, description));
  if (is_service_exist(id)) {
    logger(log_config_error, basic)
      << "Error: Service '" << description << "' on host '"
      << host_name << "' has already been defined";
    return (NULL);
  }

  // Allocate memory.
  shared_ptr<service> obj(new service, deleter::service);
  memset(obj.get(), 0, sizeof(*obj));

  try {
    // Duplicate vars.
    obj->host_id = host_id;
    obj->host_name = string::dup(host_name);
    obj->id = service_id;
    obj->description = string::dup(description);
    obj->service_check_command = string::dup(check_command);
    if (event_handler)
      obj->event_handler = string::dup(event_handler);
    if (check_period)
      obj->check_period = string::dup(check_period);
    if (timezone)
      obj->timezone = string::dup(timezone);

    obj->check_freshness = (check_freshness > 0);
    obj->check_interval = check_interval;
    obj->check_options = CHECK_OPTION_NONE;
    obj->check_type = SERVICE_CHECK_ACTIVE;
    obj->checks_enabled = (checks_enabled > 0);
    obj->check_timeout = check_timeout;
    obj->current_attempt = (initial_state == STATE_OK) ? 1 : max_attempts;
    obj->current_state = initial_state;
    obj->event_handler_enabled = (event_handler_enabled > 0);
    obj->flap_detection_enabled = (flap_detection_enabled > 0);
    obj->flap_detection_on_critical = (flap_detection_on_critical > 0);
    obj->flap_detection_on_ok = (flap_detection_on_ok > 0);
    obj->flap_detection_on_unknown = (flap_detection_on_unknown > 0);
    obj->flap_detection_on_warning = (flap_detection_on_warning > 0);
    obj->freshness_threshold = freshness_threshold;
    obj->high_flap_threshold = high_flap_threshold;
    obj->initial_state = initial_state;
    obj->is_volatile = (is_volatile > 0);
    obj->last_hard_state = initial_state;
    obj->last_state = initial_state;
    obj->low_flap_threshold = low_flap_threshold;
    obj->max_attempts = max_attempts;
    obj->modified_attributes = MODATTR_NONE;
    obj->obsess_over_service = (obsess_over_service > 0);
    obj->retry_interval = retry_interval;
    obj->should_be_scheduled = true;
    obj->state_type = HARD_STATE;

    // STATE_OK = 0, so we don't need to set state_history (memset
    // is used before).
    // for (unsigned int x(0); x < MAX_STATE_HISTORY_ENTRIES; ++x)
    //   obj->state_history[x] = STATE_OK;

    // Add new items to the configuration state.
    state::instance().services()[id] = obj;

    // Add new items to the list.
    obj->next = service_list;
    service_list = obj.get();

    // Notify event broker.
    timeval tv(get_broker_timestamp(NULL));
    broker_adaptive_service_data(
      NEBTYPE_SERVICE_ADD,
      NEBFLAG_NONE,
      NEBATTR_NONE,
      obj.get(),
      CMD_NONE,
      MODATTR_ALL,
      MODATTR_ALL,
      &tv);
  }
  catch (...) {
    obj.clear();
  }

  return (obj.get());
}

/**
 *  Get number of registered services.
 *
 *  @return Number of registered services.
 */
int get_service_count() {
  return (state::instance().services().size());
}

/**
 *  Get service by host name and service description.
 *
 *  @param[in] host_name           The host name.
 *  @param[in] service_description The service_description.
 *
 *  @return The struct service or throw exception if the
 *          service is not found.
 */
service& engine::find_service(
           std::string const& host_name,
           std::string const& service_description) {
  std::pair<std::string, std::string>
    id(std::make_pair(host_name, service_description));
  umap<std::pair<std::string, std::string>, shared_ptr<service_struct> >::const_iterator
    it(state::instance().services().find(id));
  if (it == state::instance().services().end())
    throw (engine_error() << "Service '" << service_description
           << "' on host '" << host_name << "' was not found");
  return (*it->second);
}

/**
 *  Get if service exist.
 *
 *  @param[in] id The service id.
 *
 *  @return True if the service is found, otherwise false.
 */
bool engine::is_service_exist(
       std::pair<std::string, std::string> const& id) {
  umap<std::pair<std::string, std::string>, shared_ptr<service_struct> >::const_iterator
    it(state::instance().services().find(id));
  return (it != state::instance().services().end());
}
