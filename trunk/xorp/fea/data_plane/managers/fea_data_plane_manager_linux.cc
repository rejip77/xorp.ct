// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2007 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

#ident "$XORP: xorp/fea/data_plane/managers/fea_data_plane_manager_linux.cc,v 1.1 2007/07/11 22:18:17 pavlin Exp $"

#include "fea/fea_module.h"

#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"

#include "fea/data_plane/ifconfig/ifconfig_get_getifaddrs.hh"
#include "fea/data_plane/ifconfig/ifconfig_get_ioctl.hh"
#include "fea/data_plane/ifconfig/ifconfig_get_netlink_socket.hh"
#include "fea/data_plane/ifconfig/ifconfig_get_proc_linux.hh"
#include "fea/data_plane/ifconfig/ifconfig_set_ioctl.hh"
#include "fea/data_plane/ifconfig/ifconfig_set_netlink_socket.hh"
#include "fea/data_plane/ifconfig/ifconfig_observer_netlink_socket.hh"
#include "fea/data_plane/fibconfig/fibconfig_forwarding_proc_linux.hh"
#include "fea/data_plane/fibconfig/fibconfig_entry_get_netlink_socket.hh"
#include "fea/data_plane/fibconfig/fibconfig_entry_set_netlink_socket.hh"
#include "fea/data_plane/fibconfig/fibconfig_entry_observer_netlink_socket.hh"
#include "fea/data_plane/fibconfig/fibconfig_table_get_netlink_socket.hh"
#include "fea/data_plane/fibconfig/fibconfig_table_set_netlink_socket.hh"
#include "fea/data_plane/fibconfig/fibconfig_table_observer_netlink_socket.hh"

#include "fea_data_plane_manager_linux.hh"


//
// FEA data plane manager class for Linux FEA.
//

#if 0
extern "C" FeaDataPlaneManager* create(FeaNode& fea_node)
{
    return (new FeaDataPlaneManagerLinux(fea_node));
}

extern "C" void destroy(FeaDataPlaneManager* fea_data_plane_manager)
{
    delete fea_data_plane_manager;
}
#endif // 0


FeaDataPlaneManagerLinux::FeaDataPlaneManagerLinux(FeaNode& fea_node)
    : FeaDataPlaneManager(fea_node, "Linux"),
      _ifconfig_get_ioctl(NULL)
{
}

FeaDataPlaneManagerLinux::~FeaDataPlaneManagerLinux()
{
}

int
FeaDataPlaneManagerLinux::load_plugins(string& error_msg)
{
    UNUSED(error_msg);

    if (_is_loaded_plugins)
	return (XORP_OK);

    XLOG_ASSERT(_ifconfig_get == NULL);
    XLOG_ASSERT(_ifconfig_set == NULL);
    XLOG_ASSERT(_ifconfig_observer == NULL);
    XLOG_ASSERT(_fibconfig_forwarding == NULL);
    XLOG_ASSERT(_fibconfig_entry_get == NULL);
    XLOG_ASSERT(_fibconfig_entry_set == NULL);
    XLOG_ASSERT(_fibconfig_entry_observer == NULL);
    XLOG_ASSERT(_fibconfig_table_get == NULL);
    XLOG_ASSERT(_fibconfig_table_set == NULL);
    XLOG_ASSERT(_fibconfig_table_observer == NULL);

    //
    // Load the plugins
    //
#if defined(HAVE_NETLINK_SOCKETS)
    _ifconfig_get = new IfConfigGetNetlinkSocket(*this);
#elif defined(HAVE_PROC_LINUX) && defined(HAVE_IOCTL_SIOCGIFCONF)
    // XXX: the IfConfigGetProcLinux plugin depends on IfConfigGetIoctl
    _ifconfig_get = new IfConfigGetProcLinux(*this);
    _ifconfig_get_ioctl = new IfConfigGetIoctl(*this);
#elif defined(HAVE_GETIFADDRS)
    _ifconfig_get = new IfConfigGetGetifaddrs(*this);
#elif defined(HAVE_IOCTL_SIOCGIFCONF)
    _ifconfig_get = new IfConfigGetIoctl(*this);
#endif

#if defined(HAVE_NETLINK_SOCKETS)
    _ifconfig_set = new IfConfigSetNetlinkSocket(*this);
#elif defined(HAVE_IOCTL_SIOCGIFCONF)
    _ifconfig_set = new IfConfigSetIoctl(*this);
#endif

#if defined(HAVE_NETLINK_SOCKETS)
    _ifconfig_observer = new IfConfigObserverNetlinkSocket(*this);
#endif

#if defined(HAVE_PROC_LINUX)
    _fibconfig_forwarding = new FibConfigForwardingProcLinux(*this);
#endif

#if defined(HAVE_NETLINK_SOCKETS)
    _fibconfig_entry_get = new FibConfigEntryGetNetlinkSocket(*this);
#endif

#if defined(HAVE_NETLINK_SOCKETS)
    _fibconfig_entry_set = new FibConfigEntrySetNetlinkSocket(*this);
#endif

#if defined(HAVE_NETLINK_SOCKETS)
    _fibconfig_entry_observer = new FibConfigEntryObserverNetlinkSocket(*this);
#endif

#if defined(HAVE_NETLINK_SOCKETS)
    _fibconfig_table_get = new FibConfigTableGetNetlinkSocket(*this);
#endif

#if defined(HAVE_NETLINK_SOCKETS)
    _fibconfig_table_set = new FibConfigTableSetNetlinkSocket(*this);
#endif

#if defined(HAVE_NETLINK_SOCKETS)
    _fibconfig_table_observer = new FibConfigTableObserverNetlinkSocket(*this);
#endif

    _is_loaded_plugins = true;

    return (XORP_OK);
}

int
FeaDataPlaneManagerLinux::unload_plugins(string& error_msg)
{
    if (! _is_loaded_plugins)
	return (XORP_OK);

    if (_ifconfig_get_ioctl != NULL) {
	delete _ifconfig_get_ioctl;
	_ifconfig_get_ioctl = NULL;
    }

    return (FeaDataPlaneManager::unload_plugins(error_msg));
}

int
FeaDataPlaneManagerLinux::register_plugins(string& error_msg)
{
    return (FeaDataPlaneManager::register_all_plugins(true, error_msg));
}