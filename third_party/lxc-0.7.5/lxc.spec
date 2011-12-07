#
# lxc: linux Container library
#
# (C) Copyright IBM Corp. 2007, 2008
#
# Authors:
# Daniel Lezcano <dlezcano at fr.ibm.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

Name: lxc
Version: 0.7.5
Release: 1
URL: http://lxc.sourceforge.net
Source: http://dl.sourceforge.net/sourceforge/%{name}/%{name}-%{version}.tar.gz
Summary: %{name} : Linux Container
Group: Applications/System
License: LGPL
BuildRoot: %{_tmppath}/%{name}-%{version}-build
Requires: libcap
BuildRequires: libcap libcap-devel docbook-utils

%description

The package "%{name}" provides the command lines to create and manage
containers.  It contains a full featured container with the isolation
/ virtualization of the pids, the ipc, the utsname, the mount points,
/proc, /sys, the network and it takes into account the control groups.
It is very light, flexible, and provides a set of tools around the
container like the monitoring with asynchronous events notification,
or the freeze of the container. This package is useful to create
Virtual Private Server, or to run isolated applications like bash or
sshd.

%package devel
Release: 1
Summary: development library for %{name}
Group: Development/Libraries

%description devel
The %{name}-devel package contains header files and library needed for
development of the linux containers.

%prep
%setup
%build
test "%{ksrc}" != "none" && args="--with-linuxdir=%{ksrc}"
PATH=$PATH:/usr/sbin:/sbin %configure $args
make %{?_smp_mflags}

%install
%makeinstall

find $RPM_BUILD_ROOT -type f -name '*.la' -exec rm -f {} ';'  

%clean
rm -rf %{buildroot}

%post

%files
%defattr(-,root,root)
%{_libdir}/*.so*
%{_libdir}/%{name}
%attr(4555,root,root) %{_libdir}/%{name}/lxc-init
%{_bindir}/*
%attr(4111,root,root) %{_bindir}/lxc-attach
%attr(4111,root,root) %{_bindir}/lxc-create
%attr(4111,root,root) %{_bindir}/lxc-clone
%attr(4111,root,root) %{_bindir}/lxc-start
%attr(4111,root,root) %{_bindir}/lxc-netstat
%attr(4111,root,root) %{_bindir}/lxc-unshare
%attr(4111,root,root) %{_bindir}/lxc-execute
%attr(4111,root,root) %{_bindir}/lxc-checkpoint
%attr(4111,root,root) %{_bindir}/lxc-restart
%{_mandir}/*
%{_datadir}/pkgconfig/*
%{_datadir}/doc/*

%files devel
%defattr(-,root,root)
%{_includedir}/%{name}/*
%{_libdir}/*.so.*

%changelog

* Fri Jul 23 2010 Daniel Lezcano <dlezcano@fr.ibm.com> - Version 0.7.2
- set attribute for installed files
- fix libraries installation
-

* Mon Mar 24 2009 Daniel Lezcano <daniel.lezcano@free.fr> - Version 0.6.1
- Removed capability setting, let the user to do that through "lxc-setcap"

* Mon Feb 16 2009 Daniel Lezcano <daniel.lezcano@free.fr> - Version 0.6.0
- Added more capabilities to the executables

* Sun Jan 25 2009 Daniel Lezcano <daniel.lezcano@free.fr> - Version 0.6.0
- Reduced spec file

* Sun Aug 3 2008 Daniel Lezcano <dlezcano@fr.ibm.com> - Version 0.1.0
- Initial RPM release.

# Local variables:
# mode: shell-script
# sh-shell: rpm
# end:
