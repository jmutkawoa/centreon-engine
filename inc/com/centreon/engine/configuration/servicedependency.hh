/*
** Copyright 2011-2013,2015 Merethis
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

#ifndef CCE_CONFIGURATION_SERVICEDEPENDENCY_HH
#  define CCE_CONFIGURATION_SERVICEDEPENDENCY_HH

#  include <set>
#  include "com/centreon/engine/configuration/group.hh"
#  include "com/centreon/engine/configuration/object.hh"
#  include "com/centreon/engine/opt.hh"
#  include "com/centreon/engine/namespace.hh"

CCE_BEGIN()

namespace                  configuration {
  class                    servicedependency : public object {
  public:
    enum                   action_on {
      none = 0,
      ok = (1 << 0),
      unknown = (1 << 1),
      warning = (1 << 2),
      critical = (1 << 3),
      pending = (1 << 4)
    };
    typedef                servicedependency
                           key_type;

                           servicedependency();
                           servicedependency(
                             servicedependency const& right);
                           ~servicedependency() throw ();
    servicedependency&     operator=(servicedependency const& right);
    bool                   operator==(
                             servicedependency const& right) const throw ();
    bool                   operator!=(
                             servicedependency const& right) const throw ();
    bool                   operator<(
                             servicedependency const& right) const;
    void                   check_validity() const;
    key_type const&        key() const throw ();
    void                   merge(object const& obj);
    bool                   parse(char const* key, char const* value);

    void                   dependency_period(std::string const& period);
    std::string const&     dependency_period() const throw ();
    list_string&           dependent_hosts() throw ();
    list_string const&     dependent_hosts() const throw ();
    list_string&           dependent_service_description() throw ();
    list_string const&     dependent_service_description() const throw ();
    void                   failure_options(
                             unsigned int options) throw ();
    unsigned int           failure_options() const throw ();
    void                   inherits_parent(bool inherit) throw ();
    bool                   inherits_parent() const throw ();
    list_string&           hosts() throw ();
    list_string const&     hosts() const throw ();
    list_string&           service_description() throw ();
    list_string const&     service_description() const throw ();

  private:
    struct                 setters {
      char const*          name;
      bool                 (*func)(servicedependency&, char const*);
    };

    bool                   _set_dependency_period(std::string const& value);
    bool                   _set_dependent_hostgroups(std::string const& value);
    bool                   _set_dependent_hosts(std::string const& value);
    bool                   _set_dependent_servicegroups(std::string const& value);
    bool                   _set_dependent_service_description(std::string const& value);
    bool                   _set_failure_options(std::string const& value);
    bool                   _set_inherits_parent(bool value);
    bool                   _set_hostgroups(std::string const& value);
    bool                   _set_hosts(std::string const& value);
    bool                   _set_notification_failure_options(std::string const& value);
    bool                   _set_servicegroups(std::string const& value);
    bool                   _set_service_description(std::string const& value);

    std::string            _dependency_period;
    group                  _dependent_hosts;
    group                  _dependent_service_description;
    opt<unsigned int>      _failure_options;
    group                  _hosts;
    opt<bool>              _inherits_parent;
    group                  _service_description;
    static setters const   _setters[];
  };

  typedef shared_ptr<servicedependency>    servicedependency_ptr;
  typedef std::set<servicedependency_ptr>  set_servicedependency;
}

CCE_END()

#endif // !CCE_CONFIGURATION_SERVICEDEPENDENCY_HH
