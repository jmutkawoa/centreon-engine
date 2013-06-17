/*
** Copyright 2011-2013 Merethis
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

#ifndef CCE_OBJECTS_SERVICE_HH
#  define CCE_OBJECTS_SERVICE_HH

#  include <time.h>
#  include "com/centreon/engine/common.hh"

/* Forward declaration. */
struct command_struct;
struct contactgroupsmember_struct;
struct contactsmember_struct;
struct customvariablesmember_struct;
struct host_struct;
struct objectlist_struct;
struct timeperiod_struct;

typedef struct                  service_struct {
  char*                         host_name;
  char*                         description;
  char*                         display_name;
  char*                         service_check_command;
  char*                         event_handler;
  int                           initial_state;
  double                        check_interval;
  double                        retry_interval;
  int                           max_attempts;
  int                           parallelize;
  contactgroupsmember_struct*   contact_groups;
  contactsmember_struct*        contacts;
  double                        notification_interval;
  double                        first_notification_delay;
  int                           notify_on_unknown;
  int                           notify_on_warning;
  int                           notify_on_critical;
  int                           notify_on_recovery;
  int                           notify_on_flapping;
  int                           notify_on_downtime;
  int                           stalk_on_ok;
  int                           stalk_on_warning;
  int                           stalk_on_unknown;
  int                           stalk_on_critical;
  int                           is_volatile;
  char*                         notification_period;
  char*                         check_period;
  int                           flap_detection_enabled;
  double                        low_flap_threshold;
  double                        high_flap_threshold;
  int                           flap_detection_on_ok;
  int                           flap_detection_on_warning;
  int                           flap_detection_on_unknown;
  int                           flap_detection_on_critical;
  int                           process_performance_data;
  int                           check_freshness;
  int                           freshness_threshold;
  int                           accept_passive_service_checks;
  int                           event_handler_enabled;
  int                           checks_enabled;
  int                           retain_status_information;
  int                           retain_nonstatus_information;
  int                           notifications_enabled;
  int                           obsess_over_service;
  int                           failure_prediction_enabled;
  char*                         failure_prediction_options;
  char*                         notes;
  char*                         notes_url;
  char*                         action_url;
  char*                         icon_image;
  char*                         icon_image_alt;
  customvariablesmember_struct* custom_variables;
  int                           problem_has_been_acknowledged;
  int                           acknowledgement_type;
  int                           host_problem_at_last_check;
  int                           check_type;
  int                           current_state;
  int                           last_state;
  int                           last_hard_state;
  char*                         plugin_output;
  char*                         long_plugin_output;
  char*                         perf_data;
  int                           state_type;
  time_t                        next_check;
  int                           should_be_scheduled;
  time_t                        last_check;
  int                           current_attempt;
  unsigned long                 current_event_id;
  unsigned long                 last_event_id;
  unsigned long                 current_problem_id;
  unsigned long                 last_problem_id;
  time_t                        last_notification;
  time_t                        next_notification;
  int                           no_more_notifications;
  int                           check_flapping_recovery_notification;
  time_t                        last_state_change;
  time_t                        last_hard_state_change;
  time_t                        last_time_ok;
  time_t                        last_time_warning;
  time_t                        last_time_unknown;
  time_t                        last_time_critical;
  int                           has_been_checked;
  int                           is_being_freshened;
  int                           notified_on_unknown;
  int                           notified_on_warning;
  int                           notified_on_critical;
  int                           current_notification_number;
  unsigned long                 current_notification_id;
  double                        latency;
  double                        execution_time;
  int                           is_executing;
  int                           check_options;
  int                           scheduled_downtime_depth;
  int                           pending_flex_downtime;
  int                           state_history[MAX_STATE_HISTORY_ENTRIES];
  unsigned int                  state_history_index;
  int                           is_flapping;
  unsigned long                 flapping_comment_id;
  double                        percent_state_change;
  unsigned long                 modified_attributes;

  host_struct*                  host_ptr;
  command_struct*               event_handler_ptr;
  char*                         event_handler_args;
  command_struct*               check_command_ptr;
  char*                         check_command_args;
  timeperiod_struct*            check_period_ptr;
  timeperiod_struct*            notification_period_ptr;
  objectlist_struct*            servicegroups_ptr;
  struct service_struct*        next;
  struct service_struct*        nexthash;
}                               service;

#  ifdef __cplusplus
#    include <ostream>

bool          operator==(
                service const& obj1,
                service const& obj2) throw ();
bool          operator!=(
                service_struct const& obj1,
                service_struct const& obj2) throw ();
std::ostream& operator<<(std::ostream& os, service const& obj);

#  endif // C++

#endif // !CCE_OBJECTS_SERVICE_HH


