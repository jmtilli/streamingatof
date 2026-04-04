#!/bin/sh

if [ '!' -f "libstreamingatof.a" -o '!' -f "libstreamingatof.so" -o '!' -f "libstreamingatof.so.1" ]; then
  echo "streamingatof not made"
  exit 1
fi

PREFIX="$1"

if [ "a$PREFIX" = "a" ]; then
  PREFIX=/usr/local
fi

P="$PREFIX"
H="`hostname`"

if [ '!' -w "$P" ]; then
  echo "No write permissions to $P"
  exit 1
fi
if [ '!' -d "$P" ]; then
  echo "Not a valid directory: $P"
  exit 1
fi

instlib()
{
  if [ -e "$P/lib/$1" ]; then
    ln "$P/lib/$1" "$P/lib/.$1.safinstold.$$.$H" || exit 1
  fi
  cp "$1" "$P/lib/.$1.safinstnew.$$.$H" || exit 1
  mv "$P/lib/.$1.safinstnew.$$.$H" "$P/lib/$1" || exit 1
  if [ -e "$P/lib/.$1.safinstold.$$.$H" ]; then
    # If you mount binaries across NFS, and run this command on the NFS server,
    # you might want to comment out this rm command.
    rm "$P/lib/.$1.safinstold.$$.$H" || exit 1
  fi
}
instinc()
{
  if [ -e "$P/include/$1" ]; then
    ln "$P/include/$1" "$P/include/.$1.safinstold.$$.$H" || exit 1
  fi
  cp "$1" "$P/include/.$1.safinstnew.$$.$H" || exit 1
  mv "$P/include/.$1.safinstnew.$$.$H" "$P/include/$1" || exit 1
  if [ -e "$P/include/.$1.safinstold.$$.$H" ]; then
    # If you mount binaries across NFS, and run this command on the NFS server,
    # you might want to comment out this rm command.
    rm "$P/include/.$1.safinstold.$$.$H" || exit 1
  fi
}
instsym()
{
  if [ "`readlink "$P/lib/$1"`" != "libstreamingatof.so.1" ]; then
    ln -s libstreamingatof.so.1 "$P/lib/.$1.safinstnew.$$.$H" || exit 1
    mv "$P/lib/.$1.safinstnew.$$.$H" "$P/lib/$1" || exit 1
  fi
}

instlib libstreamingatof.a
instlib libstreamingatof.so.1
instsym libstreamingatof.so

instinc streamingatof.h

echo "All done, streamingatof has been installed to $P"
