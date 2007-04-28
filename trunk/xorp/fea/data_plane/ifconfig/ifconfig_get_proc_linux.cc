// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2007 International Computer Science Institute
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

#ident "$XORP: xorp/fea/forwarding_plane/ifconfig/ifconfig_get_proc_linux.cc,v 1.2 2007/04/26 06:29:46 pavlin Exp $"

#include "fea/fea_module.h"

#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"
#include "libxorp/ether_compat.h"
#include "libxorp/ipvx.hh"

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ifdef HAVE_NET_IF_H
#include <net/if.h>
#endif

#include "fea/ifconfig.hh"
#include "fea/ifconfig_get.hh"
#include "fea/kernel_utils.hh"


const string IfConfigGetProcLinux::PROC_LINUX_NET_DEVICES_FILE_V4 = "/proc/net/dev";
const string IfConfigGetProcLinux::PROC_LINUX_NET_DEVICES_FILE_V6 = "/proc/net/if_inet6";

//
// Get information about network interfaces from the underlying system.
//
// The mechanism to obtain the information is Linux's /proc/net/dev (for IPv4)
// or /proc/net/if_inet6 (for IPv6).
//

IfConfigGetProcLinux::IfConfigGetProcLinux(IfConfig& ifconfig)
    : IfConfigGet(ifconfig)
{
#ifdef HAVE_PROC_LINUX
    register_ifconfig_primary();
#endif
}

IfConfigGetProcLinux::~IfConfigGetProcLinux()
{
    string error_msg;

    if (stop(error_msg) != XORP_OK) {
	XLOG_ERROR("Cannot stop the Linux's /proc mechanism to get "
		   "information about network interfaces from the underlying "
		   "system: %s",
		   error_msg.c_str());
    }
}

int
IfConfigGetProcLinux::start(string& error_msg)
{
    if (_is_running)
	return (XORP_OK);

    // XXX: this method relies on the ioctl() method
    if (ifconfig().ifconfig_get_ioctl().start(error_msg) < 0)
	return (XORP_ERROR);

    _is_running = true;

    return (XORP_OK);
}

int
IfConfigGetProcLinux::stop(string& error_msg)
{
    if (! _is_running)
	return (XORP_OK);

    // XXX: this method relies on the ioctl() method
    if (ifconfig().ifconfig_get_ioctl().stop(error_msg) < 0)
	return (XORP_ERROR);

    _is_running = false;

    return (XORP_OK);
}

bool
IfConfigGetProcLinux::pull_config(IfTree& iftree)
{
    return read_config(iftree);
}

#ifndef HAVE_PROC_LINUX
bool
IfConfigGetProcLinux::read_config(IfTree& )
{
    return false;
}

#else // HAVE_PROC_LINUX

static char* get_proc_linux_iface_name(char* name, char* p);
static bool proc_read_ifconf_linux(IfConfig& ifconfig, IfTree& iftree,
				   int family,
				   const string& proc_linux_net_device_file);
static bool if_fetch_linux_v4(IfConfig& ifconfig, IfTree& it,
			      const string& proc_linux_net_device_file);

#ifdef HAVE_IPV6
static bool if_fetch_linux_v6(IfConfig& ifconfig, IfTree& it,
			      const string& proc_linux_net_device_file);
#endif

bool
IfConfigGetProcLinux::read_config(IfTree& iftree)
{
    // XXX: this method relies on the ioctl() method
    ifconfig().ifconfig_get_ioctl().pull_config(iftree);
    
    //
    // The IPv4 information
    //
    if (ifconfig().have_ipv4()) {
	if (proc_read_ifconf_linux(ifconfig(), iftree, AF_INET,
				   PROC_LINUX_NET_DEVICES_FILE_V4)
	    != true)
	    return false;
    }
    
#ifdef HAVE_IPV6
    //
    // The IPv6 information
    //
    if (ifconfig().have_ipv6()) {
	if (proc_read_ifconf_linux(ifconfig(), iftree, AF_INET6,
				   PROC_LINUX_NET_DEVICES_FILE_V6)
	    != true)
	    return false;
    }
#endif // HAVE_IPV6
    
    return true;
}

//
// Derived from Redhat Linux ifconfig code
//

static bool
proc_read_ifconf_linux(IfConfig& ifconfig, IfTree& iftree, int family,
		       const string& proc_linux_net_device_file)
{
    switch (family) {
    case AF_INET:
	//
	// The IPv4 information
	//
	if_fetch_linux_v4(ifconfig, iftree, proc_linux_net_device_file);
	break;

#ifdef HAVE_IPV6
	//
	// The IPv6 information
	//
    case AF_INET6:
	if_fetch_linux_v6(ifconfig, iftree, proc_linux_net_device_file);
	break;
#endif // HAVE_IPV6

    default:
	XLOG_UNREACHABLE();
	break;
    }
    
    return true;
}

static char*
get_proc_linux_iface_name(char* name, char* p)
{
    while (xorp_isspace(*p))
	p++;
    if (*p == '\0')
	return (NULL);

    while (*p) {
	if (xorp_isspace(*p))
	    break;
	if (*p == ':') {	/* could be an alias */
	    char *dot = p, *dotname = name;
	    *name++ = *p++;
	    while (xorp_isdigit(*p))
		*name++ = *p++;
	    if (*p != ':') {	/* it wasn't, backup */
		p = dot;
		name = dotname;
	    }
	    if (*p == '\0')
		return (NULL);
	    p++;
	    break;
	}
	*name++ = *p++;
    }
    *name++ = '\0';
    return (p);
}

// 
// IPv4 interface info fetch
//
// We get only the list of interfaces from /proc/net/dev.
// Then, for each interface in that list we check if it is already
// in the IfTree that is partially filled-in with
// IfConfigGetIoctl::read_config().
// If the interface is in that list, then we ignore it. If the interface is
// NOT in that list, then we call IfConfigGet::parse_buffer_ifreq()
// to get the rest of the information about it (such as the interface hardware
// address, MTU, etc) by calling various ioctl()'s.
//
// Why do we need to do this in such complicated way?
// Because on Linux ioctl(SIOCGIFCONF) doesn't return information about
// tunnel interfaces, and because /proc/net/dev doesn't return information
// about IP address aliases (e.g., interface names like eth0:0).
// Hence, we have to follow the model used by Linux's ifconfig
// (as part of the net-tools collection) to merge the information from
// both mechanisms. Enjoy!
//
static bool
if_fetch_linux_v4(IfConfig& ifconfig, IfTree& it,
		  const string& proc_linux_net_device_file)
{
    FILE *fh;
    char buf[512];
    char *s, ifname[IFNAMSIZ];

    fh = fopen(proc_linux_net_device_file.c_str(), "r");
    if (fh == NULL) {
	XLOG_FATAL("Cannot open file %s for reading: %s",
		   proc_linux_net_device_file.c_str(), strerror(errno)); 
	return false;
    }
    fgets(buf, sizeof(buf), fh);	// lose starting 2 lines of comments
    s = get_proc_linux_iface_name(ifname, buf);
    if (strcmp(ifname, "Inter-|") != 0) {
	XLOG_ERROR("%s: improper file contents",
		   proc_linux_net_device_file.c_str());
	fclose(fh);
	return false;
    }
    fgets(buf, sizeof(buf), fh);
    s = get_proc_linux_iface_name(ifname, buf);    
    if (strcmp(ifname, "face") != 0) {
	XLOG_ERROR("%s: improper file contents",
		   proc_linux_net_device_file.c_str());
	fclose(fh);
	return false;
    }
    
    while (fgets(buf, sizeof(buf), fh) != NULL) {
	//
	// Get the interface name
	//
	s = get_proc_linux_iface_name(ifname, buf);
	if (s == NULL) {
	    XLOG_ERROR("%s: cannot get interface name for line %s",
		       proc_linux_net_device_file.c_str(), buf);
	    continue;
	}
	
	if (it.find_interface(ifname) != NULL) {
	    // Already have this interface. Ignore it.
	    continue;
	}

	struct ifreq ifreq;
	memset(&ifreq, 0, sizeof(ifreq));
	strncpy(ifreq.ifr_name, ifname, sizeof(ifreq.ifr_name) - 1);
	vector<uint8_t> buffer(sizeof(struct ifreq));
	memcpy(&buffer[0], &ifreq, sizeof(ifreq));

	ifconfig.ifconfig_get_ioctl().parse_buffer_ifreq(it, AF_INET, buffer);
    }
    if (ferror(fh)) {
	XLOG_ERROR("%s read failed: %s",
		   proc_linux_net_device_file.c_str(), strerror(errno));
    }
    
    // Clean up
    fclose(fh);
    
    return true;
}

// 
// IPv6 interface info fetch
//
// Unlike the IPv4 case (see the comments about if_fetch_linux_v4()),
// we get the list of interfaces and all the IPv6 address information
// from /proc/net/if_inet6.
// Then, we use ioctl()'s to obtain information such as interface
// hardware address, MTU, etc.
//
// The reason that we do NOT call IfConfigGet::parse_buffer_ifreq()
// to fill-in the missing information is because in Linux the in6_ifreq
// structure has completely different format from the IPv4 equivalent
// of "struct ifreq" (i.e., there is no in6_ifreq.ifr_name field), and
// that structure is completely different from the BSD structure with
// the same name. No comment.
//
#ifdef HAVE_IPV6
static bool
if_fetch_linux_v6(IfConfig& ifconfig, IfTree& it,
		  const string& proc_linux_net_device_file)
{
    FILE *fh;
    char devname[IFNAMSIZ+20+1];
    char addr6p[8][5], addr6[40];
    int plen, scope, dad_status, if_idx;
    struct ifreq ifreq;
    XorpFd sock;
    
    fh = fopen(proc_linux_net_device_file.c_str(), "r");
    if (fh == NULL) {
	XLOG_FATAL("Cannot open file %s for reading: %s",
		   proc_linux_net_device_file.c_str(), strerror(errno)); 
	return false;
    }
    
    sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock < 0) {
	XLOG_FATAL("Could not initialize ioctl() socket");
	fclose(fh);
	return false;
    }
    
    while (fscanf(fh, "%4s%4s%4s%4s%4s%4s%4s%4s %02x %02x %02x %02x %20s\n",
		  addr6p[0], addr6p[1], addr6p[2], addr6p[3],
		  addr6p[4], addr6p[5], addr6p[6], addr6p[7],
		  &if_idx, &plen, &scope, &dad_status, devname) != EOF) {
	u_short if_index = 0;
	string if_name, alias_if_name;
	
	//
	// Get the interface name
	//
	char tmp_if_name[IFNAMSIZ+1];
	strncpy(tmp_if_name, devname, sizeof(tmp_if_name) - 1);
	tmp_if_name[sizeof(tmp_if_name) - 1] = '\0';
	char *cptr;
	if ( (cptr = strchr(tmp_if_name, ':')) != NULL) {
	    // Replace colon with null. Needed because in Solaris and Linux
	    // the interface name changes for aliases.
	    *cptr = '\0';
	}
	if_name = string(devname);
	alias_if_name = string(tmp_if_name);
	debug_msg("interface: %s\n", if_name.c_str());
	debug_msg("alias interface: %s\n", alias_if_name.c_str());
	
	//
	// Get the physical interface index
	//
	if_index = if_idx;
	if (if_index == 0) {
	    XLOG_FATAL("Could not find physical interface index "
		       "for interface %s",
		       if_name.c_str());
	}
	debug_msg("interface index: %d\n", if_index);
	
	//
	// Add the interface (if a new one)
	//
	ifconfig.map_ifindex(if_index, alias_if_name);
	it.add_if(alias_if_name);
	IfTreeInterface* ifp = it.find_interface(alias_if_name);
	XLOG_ASSERT(ifp != NULL);

	//
	// Set the physical interface index for the interface
	//
	ifp->set_pif_index(if_index);

	//
	// Get the MAC address
	//
	do {
#ifdef SIOCGIFHWADDR
	    memset(&ifreq, 0, sizeof(ifreq));
	    strncpy(ifreq.ifr_name, if_name.c_str(),
		    sizeof(ifreq.ifr_name) - 1);
	    if (ioctl(sock, SIOCGIFHWADDR, &ifreq) < 0) {
		XLOG_ERROR("ioctl(SIOCGIFHWADDR) for interface %s failed: %s",
			   if_name.c_str(), strerror(errno));
	    } else {
		struct ether_addr ea;
		memcpy(&ea, ifreq.ifr_hwaddr.sa_data, sizeof(ea));
		ifp->set_mac(EtherMac(ea));
		break;
	    }
#endif // SIOCGIFHWADDR
	    
	    break;
	} while (false);
	debug_msg("MAC address: %s\n", ifp->mac().str().c_str());
	
	//
	// Get the MTU
	//
	int mtu = 0;
	memset(&ifreq, 0, sizeof(ifreq));
	strncpy(ifreq.ifr_name, if_name.c_str(), sizeof(ifreq.ifr_name) - 1);
	if (ioctl(sock, SIOCGIFMTU, &ifreq) < 0) {
	    XLOG_ERROR("ioctl(SIOCGIFMTU) for interface %s failed: %s",
		       if_name.c_str(), strerror(errno));
	} else {
	    mtu = ifreq.ifr_mtu;
	}
	ifp->set_mtu(mtu);
	debug_msg("MTU: %d\n", ifp->mtu());
	
	//
	// Get the flags
	//
	int flags = 0;
	// int flags6 = dad_status;
	memset(&ifreq, 0, sizeof(ifreq));
	strncpy(ifreq.ifr_name, if_name.c_str(), sizeof(ifreq.ifr_name) - 1);
	if (ioctl(sock, SIOCGIFFLAGS, &ifreq) < 0) {
	    XLOG_ERROR("ioctl(SIOCGIFFLAGS) for interface %s failed: %s",
		       if_name.c_str(), strerror(errno));
	} else {
	    flags = ifreq.ifr_flags;
	}
	ifp->set_if_flags(flags);
	ifp->set_enabled(flags & IFF_UP);
	debug_msg("enabled: %s\n", ifp->enabled() ? "true" : "false");

	//
	// Get the link status
	//
	bool no_carrier = false;
	if ((flags & IFF_UP) && !(flags & IFF_RUNNING))
	    no_carrier = true;
	else
	    no_carrier = false;
	ifp->set_no_carrier(no_carrier);
	debug_msg("no_carrier: %s\n", ifp->no_carrier() ? "true" : "false");
	
	// XXX: vifname == ifname on this platform
	ifp->add_vif(alias_if_name);
	IfTreeVif* vifp = ifp->find_vif(alias_if_name);
	XLOG_ASSERT(vifp != NULL);
	
	//
	// Set the physical interface index for the vif
	//
	vifp->set_pif_index(if_index);
	
	//
	// Set the vif flags
	//
	vifp->set_enabled(ifp->enabled() && (flags & IFF_UP));
	vifp->set_broadcast(flags & IFF_BROADCAST);
	vifp->set_loopback(flags & IFF_LOOPBACK);
	vifp->set_point_to_point(flags & IFF_POINTOPOINT);
	vifp->set_multicast(flags & IFF_MULTICAST);
	debug_msg("vif enabled: %s\n", vifp->enabled() ? "true" : "false");
	debug_msg("vif broadcast: %s\n", vifp->broadcast() ? "true" : "false");
	debug_msg("vif loopback: %s\n", vifp->loopback() ? "true" : "false");
	debug_msg("vif point_to_point: %s\n", vifp->point_to_point() ? "true"
		  : "false");
	debug_msg("vif multicast: %s\n", vifp->multicast() ? "true" : "false");
	
	//
	// Get the IP address, netmask, P2P destination
	//
	// The default values
	IPv6 lcl_addr;
	IPv6 subnet_mask;
	IPv6 peer_addr;
	bool has_peer_addr = false;
	
	// Get the IP address
	snprintf(addr6, sizeof(addr6), "%s:%s:%s:%s:%s:%s:%s:%s",
		 addr6p[0], addr6p[1], addr6p[2], addr6p[3],
		 addr6p[4], addr6p[5], addr6p[6], addr6p[7]);
	lcl_addr = IPv6(addr6);
	lcl_addr = kernel_adjust_ipv6_recv(lcl_addr);
	debug_msg("IP address: %s\n", lcl_addr.str().c_str());
	
	// Get the netmask
	subnet_mask = IPv6::make_prefix(plen);
	debug_msg("IP netmask: %s\n", subnet_mask.str().c_str());
	
	// Get the p2p address
	if (vifp->point_to_point()) {
	    XLOG_FATAL("IPv6 point-to-point address unimplemented");
	    has_peer_addr = true;
	    debug_msg("Peer address: %s\n", peer_addr.str().c_str());
	}
	
	debug_msg("\n");	// put an empty line between interfaces
	
	// Add the address
	vifp->add_addr(lcl_addr);
	IfTreeAddr6* ap = vifp->find_addr(lcl_addr);
	XLOG_ASSERT(ap != NULL);
	ap->set_enabled(vifp->enabled() && (flags & IFF_UP));
	ap->set_loopback(vifp->loopback() && (flags & IFF_LOOPBACK));
	ap->set_point_to_point(vifp->point_to_point() && (flags & IFF_POINTOPOINT));
	ap->set_multicast(vifp->multicast() && (flags & IFF_MULTICAST));
	
	ap->set_prefix_len(subnet_mask.mask_len());
	if (ap->point_to_point())
	    ap->set_endpoint(peer_addr);
    }
    
    if (ferror(fh)) {
	XLOG_ERROR("%s read failed: %s",
		   proc_linux_net_device_file.c_str(), strerror(errno));
    }
    
    close(sock);
    fclose(fh);
    
    return true;
}
#endif // HAVE_IPV6

#endif // HAVE_PROC_LINUX
