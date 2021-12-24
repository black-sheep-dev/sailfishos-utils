Name:       sailfish-utils

Summary:    Collection of different utils for SailfishOS app development 
Version:    1.0
Release:    1
Group:      System/Libraries
License:    MIT
URL:        https://github.com/black-sheep-dev/sailfishos-utils
Source0:    %{name}-%{version}.tar.bz2

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(openssl)

%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}

%description
Collection of different utils for SailfishOS app development 

%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

%files
%defattr(-,root,root,-)
%{_libdir}/libsailfishos-utils.a
%{_includedir}/sailfishos-utils/*.h
