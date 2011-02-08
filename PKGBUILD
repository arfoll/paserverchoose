# Maintainer: Brendan Le Foll <brendan@fridu.net>
pkgname=paserverchoose
pkgver=0.1.1
pkgrel=1
pkgdesc="CLI tool to choose pulseaudio servers"
arch=('i686' 'x86_64')
url=""
license=('GPLv2')
groups=()
depends=()
makedepends=('avahi' 'x11' 'pulseaudio')
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=($pkgname-$pkgver.tar.gz)
noextract=()
md5sums=() #generate with 'makepkg -g'

build() {
  cd "$srcdir/$pkgname-$pkgver"

  ./configure --prefix=/usr
  make
}

package() {
  cd "$srcdir/$pkgname-$pkgver"

  make DESTDIR="$pkgdir/" install
}

