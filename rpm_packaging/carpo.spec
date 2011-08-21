%define sourceversion 0.3.0
%define relcount 2
%define assume_meego 0

Name:    carpo
Version: %{sourceversion}
Release: %{relcount}%{dist}
Summary: Carp RSS/Atom Reader

License: GPLv2
Group:   Applications/Network
Packager: Bernd Stramm <bernd.stramm@gmail.com>
Url:     http://carpo.sourceforge.net/
Source: %{name}-%{version}.tar.gz
Source2: linux/%{name}.desktop
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

# TODO: patches (?)


#Obsoletes:

#Provides: 

BuildRequires: pkgconfig(QtNetwork) >= 4.7
BuildRequires: pkgconfig(QtWebKit) >= 4.7
BuildRequires: pkgconfig(QtDeclarative) >= 4.7
BuildRequires: desktop-file-utils
BuildRequires: gcc-c++

%description
RSS Reader


%prep
%setup

%build
sh configure
make 

%install

rm -rf $RPM_BUILD_ROOT
install -p -d $RPM_BUILD_ROOT%{_bindir}
install -p -m 755 bin/%{name} $RPM_BUILD_ROOT%{_bindir}
install -p -d $RPM_BUILD_ROOT%{_datadir}/pixmaps
install -p -m 755 %{name}.png $RPM_BUILD_ROOT%{_datadir}/pixmaps
desktop-file-install                                    \
--dir=${RPM_BUILD_ROOT}%{_datadir}/applications         \
%{SOURCE2}

%clean
rm -rf %{buildroot}


%post 

%postun 


%files
%defattr(-,root,root,-)
%doc README
%doc COPYRIGHT
%doc LICENSE.txt
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%changelog
* Fri Jul 21 2011 Bernd Stramm <bernd.stramm@gmail.com> - 0.2.10-1
- change label on button
- fix changelog email format

* Sun May 22 2011 Bernd Stramm <bernd.stramm@gmail.com> - 0.2.9-1
- fix icon issues
- build distro flag for arch linux

* Sat May 21 2011 Bernd Stramm <bernd.stramm@gmail.com> - 0.2.8-1
- remove QMainWindow
- simplify QML dependencies; remove maitain image; remove superfluous gesture 

* Sat May 07 2011 Bernd Stramm <bernd.stramm@gmail.com> - 0.2.6-1
- private browsing set
- shinier buttons

* Tue Apr 26 2011 Bernd Stramm <bernd.stramm@gmail.com> - 0.2.5-1
- move web nav to top when index area minimized

* Tue Apr 26 2011 Bernd Stramm <bernd.stramm@gmail.com> - 0.2.4-1
- nicer URL forwarding functions

* Thu Apr 07 2011 Bernd Stramm <bernd.stramm@gmail.com> - 0.2.3-1
- follow forwarded probe requests
- fix label on delete feed button

* Wed Apr 06 2011 Bernd Stramm <bernd.stramm@gmail.com> - 0.2.2-1
- provide reset-to-defaults in settings
- make max age for storysig configurable in settings
- support mouse wheel for scrolling webview
- add a load indicator for getting feed index
- clean up to save a little memory
- implement swipe for news stream as well as topic list
- make news stream poll time configurable
- display story title when story selected from news stream
- allow selecting text from web pages after double-click
- remove display of unused Nick field

* Mon Mar 28 2011 Bernd Stramm <bernd.stramm@gmail.com> - 0.2.0-1
- initial release


