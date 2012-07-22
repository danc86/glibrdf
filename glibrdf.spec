
Name:           glibrdf
Version:        0.0.0
Release:        1%{?dist}
Summary:        GLib wrapper for Redland RDF library

Group:          System Environment/Libraries
License:        GPLv3+
URL:            https://github.com/danc86/glibrdf/
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(gobject-2.0)
BuildRequires:  pkgconfig(redland)
BuildRequires:  pkgconfig(raptor2)

%package devel
Group:          Development/Libraries
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description
A small wrapper around the Redland RDF library (librdf) to make it work nicely
with GLib and Vala and the rest of the GNOME ecosystem.

%description devel
This package contains files needed for developing software that uses
%{name}.

%prep
%setup -q

%build
make PREFIX="%{_prefix}" LIBDIR="%{_libdir}" %{?_smp_mflags}

%install
make PREFIX="%{_prefix}" LIBDIR="%{_libdir}" DESTDIR="%{buildroot}" install

%files
%doc README.markdown COPYING
%{_libdir}/libglibrdf.so.0*

%files devel
%{_includedir}/%{name}.h
%{_libdir}/libglibrdf.so
%{_libdir}/pkgconfig/%{name}.pc
%{_datadir}/vala/vapi/%{name}.vapi

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%changelog
* Sun Jul 22 2012 Dan Callaghan <djc@djc.id.au> 0.0.0-1
- development version
