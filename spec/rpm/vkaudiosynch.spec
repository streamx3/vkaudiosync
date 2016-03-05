Name: vkaudiosync
Version: 0.16b
Release: 16
Summary: audio synchronizer for vk.com
Summary(ru): Программа для синхронизации аудио записей из ВКонтакта с директорией
Group: Internet
License: GNU GPLv3
URL: https://bitbucket.org/denis_medved/vkaudiosync/src
Source: %{name}.tar.gz
BuildRoot: /tmp/%{name}
BuildRequires: gcc
Requires: qt >= 4.8.1 qtwebkit openssl

%description

%prep
%setup -n vkaudiosync

%build
cd src
qmake
make

%install
cd src
INSTALL_ROOT=$RPM_BUILD_ROOT make install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%attr(755, root, root) /usr/bin/vkaudiosync
%defattr(644, root, root)
/usr/share/applications/vkaudiosync.desktop
/usr/share/icons/hicolor/64x64/apps/vkaudiosync.png
/usr/share/vkaudiosync/translations/main_en.qm
/usr/share/vkaudiosync/translations/main_ru.qm

