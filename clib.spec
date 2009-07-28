Name:           clib
Version:        0.1.99
Release:        2%{?dist}
Summary:        Development files for the clib library

Group:          Development/Libraries
License:        2-clause BSD
URL:            http://github.com/caoimhechaos/clib/
Source0:        clib-%{version}.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
clib is a development library which provides the end user with hashes,
arrays and other features that are very useful for programming in the
C programming language.

%package        devel
Summary:        Development files for the clib library
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.


%prep
%setup -q


%build
%configure --disable-static
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'


%clean
rm -rf $RPM_BUILD_ROOT


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
%doc
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
%doc
%{_includedir}/*
%{_libdir}/*.so


%changelog
libclib (0.1.99-2) unstable; urgency=low

  * Added function to convert host:port strings to socket addresses for
    connect, bind etc.

 -- Caoimhe Chaos <caoimhechaos@protonmail.com>  Tue, 28 Jul 2009 14:02:53 +0200

libclib (0.1.99-1) unstable; urgency=low

  * Initial release for RedHat
  * Corrected return types for default hash functions
  * Fixed array implementation

 -- Caoimhe Chaos <caoimhechaos@protonmail.com>  Tue, 21 Jul 2009 08:23:33 +0200

