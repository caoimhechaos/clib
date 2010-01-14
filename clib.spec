Name:           clib
Version:        0.2.2
Release:        1%{?dist}
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
* Thu Jan 14 2010 - caoimhechaos@protonmail.com
- New stable release 0.2.2 (Bugfix release).
- Fix for the clear function from Andreas Bartsch.
- Fix for the array initialization from Andreas Bartsch.

* Thu Nov 12 2009 - caoimhechaos@protonmail.com
- New stable release 0.2.1.
- Fixed some memory leaks in the test suite.
- Fixed off-by-one error which prevented c_array_pop() from working and
  added a test case.
- Fixed the case of reallocation to 0 in _c_array_delete(), making it
  possible to empty an array.

* Wed Jul 29 2009 - caoimhechaos@protonmail.com
- New stable release 0.2.
- Added destructors to the array API to match the hash API.

* Tue Jul 28 2009 - caoimhechaos@protonmail.com
- Added function to convert host:port strings to socket addresses for
  connect, bind etc.

* Tue Jul 21 2009 - caoimhechaos@protonmail.com
- Initial release for RedHat
- Corrected return types for default hash functions
- Fixed array implementation
