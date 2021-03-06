Alonexec
--------

So. What's Alonexec ? Sometimes, I work for non-geek people. So,
sometimes I really need to deliver only one executable to the final
end-user. I want the end-user to do just one click for starting the
application. It seems obvious for little applications, but for big ones,
there is not just one executable. That's the purpose of Alonexec. For
instance, my software needs an sqlite database, and some little others
files to work. Alonexec will "pack" everything in only one binary. When
started, everything will be extracted in a temporary directory, and the
binaries you want, will be executed directly after that. Don't worry,
it's pretty fast.

### Note that Alonexec is under active development and shouldn't be used in a production environment.

How to use it ?
---------------

Alonexec is not very complex to use. I will use an example, it will be
easier.

      ./alonexec
        ./linux.alonexec
        ./tcc
        ../rsrc/spec.h

The linux.alonexec looks like that (it's just an example, it's for
alonexecing alonexec on a Linux platform):

      (alonexec
         (sourcepath "alonexec")
         (destpath "alonexec")
         (mode "rwxr-xr-x")
         (autostart true))

        (linux.alonexec
         (sourcepath "linux.alonexec")
         (destpath "linux.alonexec")
         (mode "rwxr-xr-x")
         (autostart false))

        (spec.h
         (sourcepath "../rsrc/spec.h")
         (destpath "../rsrc/spec.h")
         (mode "rwxr-xr-x")
         (autostart false))

        (tcc
         (sourcepath "tcc")
         (destpath "tcc")
         (mode "rwxr-xr-x")
         (autostart false))

So, as you can see, the template file uses a s-expression syntax
(yeah!). You must pass this type of template as first argument to
alonexec. Sourcepath is the path (relative to alonexec binary or
absolute path) of the file you want to pack. Destpath will be used after
extraction in the temporary directory. Most of time (it's a specific
case here) you DON'T want '..' paths there. Mode is the permissions
which will be setted on the file before execution. Autostart, a boolean
value, if you want to execute the file after extraction or not. Alonexec
will print the full list of generated .c in this manner:

       pcboy@home build % alonexec test.alonexec
        Packing test
        /tmp/alonexectmp.223976034/alonexecgen.c
        /tmp/alonexectmp.223976034/test.c

Now you just have to compile these .c files with your favorite compiler.
You can also use alonexecc which call cc automatically on the files.

       pcboy@home build % ./alonexec
        pcboy@home build % alonexecc
        /usr/local/bin/alonexecc file.alonexec destbinary
        pcboy@home build % alonexecc test.alonexec tartine
        /tmp/alonexectmp.642142850/alonexecgen.c
        /tmp/alonexectmp.642142850/test.c
        pcboy@home build % ls -l tartine
        -rwxr-xr-x 1 pcboy pcboy 19125 Jul 22 04:03 tartine

### Need any aguments ?

Because I wanted to keep that as KISS as possible, there is no direct
way for adding arguments to executables for autostarting. But don't
worry, you can do it easily. Look at this template:

        (program1
         (sourcepath "program1")
         (destpath "program1")
         (mode "r-xr-xr-x)
         (autostart false))

        (launcher
         (sourcepath "launcher.sh")
         (destpath "launcher.sh")
         (mode "r-xr-xr-x")
         (autostart true))

program1 is the program you want to run. But as you can see, I didn't
set the autostart flag. I've included another script named launcher.sh
**with** the autostart flag. In launcher.sh:

        #!/usr/bin/env sh
        program1 first_arg second_arg

When the final binary generated by alonexec will be launched, the script
launcher.sh will starts the binary program1 with good arguments. Easy.

Examples
--------

Just a little example showing the possibilities. You want to make a
binary which shows automatically a video after clicking on it ? In a way
that works everywhere (on the same OS of course) without dependencies ?
Template:

        (video
         (sourcepath "video.avi")
         (destpath "video.avi")
         (mode "r--r--r--")
         (autostart false))

        (player
         (sourcepath "player")
         (destpath "player")
         (mode "r-xr-xr-x")
         (autostart false))

        (launcher
         (sourcepath "launcher.sh")
         (destpath "launcher.sh")
         (mode "r-xr-xr-x")
         (autostart true))

Player is a statically compiled mplayer (for instance). And here is the
launcher script:

        #!/usr/bin/env sh
        ./player video.avi

Just a click, and BAM! self-executable video. (don't do that with too
big videos of course)

Another one. Here is a ldd on a library:

        linux-gate.so.1 =>  (0xffffe000)
        libQtGui.so.4 => /usr/lib/qt4/libQtGui.so.4 (0xb6c62000)
        libpng14.so.14 => /usr/lib/libpng14.so.14 (0xb6c3c000)
        libSM.so.6 => /usr/lib/libSM.so.6 (0xb6c33000)
        libICE.so.6 => /usr/lib/libICE.so.6 (0xb6c1a000)
        libXrender.so.1 => /usr/lib/libXrender.so.1 (0xb6c0f000)
        libXrandr.so.2 => /usr/lib/libXrandr.so.2 (0xb6c06000)
        libXcursor.so.1 => /usr/lib/libXcursor.so.1 (0xb6bfb000)
        libXfixes.so.3 => /usr/lib/libXfixes.so.3 (0xb6bf5000)
        libfreetype.so.6 => /usr/lib/libfreetype.so.6 (0xb6b6f000)
        libfontconfig.so.1 => /usr/lib/libfontconfig.so.1 (0xb6b3e000)
        libXext.so.6 => /usr/lib/libXext.so.6 (0xb6b2e000)
        libX11.so.6 => /usr/lib/libX11.so.6 (0xb6a11000)
        libQtSql.so.4 => /usr/lib/qt4/libQtSql.so.4 (0xb69c5000)
        libQtNetwork.so.4 => /usr/lib/qt4/libQtNetwork.so.4 (0xb6880000)
        libssl.so.0.9.8 => /usr/lib/libssl.so.0.9.8 (0xb6837000)
        libQtCore.so.4 => /usr/lib/qt4/libQtCore.so.4 (0xb657e000)
        libz.so.1 => /lib/libz.so.1 (0xb656b000)
        libgthread-2.0.so.0 => /usr/lib/libgthread-2.0.so.0 (0xb6565000)
        libglib-2.0.so.0 => /usr/lib/libglib-2.0.so.0 (0xb647b000)
        librt.so.1 => /lib/librt.so.1 (0xb6472000)
        libpthread.so.0 => /lib/libpthread.so.0 (0xb6459000)
        libdl.so.2 => /lib/libdl.so.2 (0xb6454000)
        libstdc++.so.6 => /usr/local/lib/libstdc++.so.6 (0xb6358000)
        libm.so.6 => /lib/libm.so.6 (0xb6332000)
        libgcc_s.so.1 => /usr/local/lib/libgcc_s.so.1 (0xb6314000)
        libc.so.6 => /lib/libc.so.6 (0xb61cb000)
        libpng12.so.0 => /usr/lib/libpng12.so.0 (0xb61a5000)
        libgobject-2.0.so.0 => /usr/lib/libgobject-2.0.so.0 (0xb6164000)
        libXinerama.so.1 => /usr/lib/libXinerama.so.1 (0xb6160000)
        libuuid.so.1 => /lib/libuuid.so.1 (0xb615b000)
        libexpat.so.1 => /usr/lib/libexpat.so.1 (0xb6130000)
        libxcb.so.1 => /usr/lib/libxcb.so.1 (0xb6114000)
        libcrypto.so.0.9.8 => /usr/lib/libcrypto.so.0.9.8 (0xb5fc5000)
        /lib/ld-linux.so.2 (0xb76e7000)
        libXau.so.6 => /usr/lib/libXau.so.6 (0xb5fc1000)
        libXdmcp.so.6 => /usr/lib/libXdmcp.so.6 (0xb5fbb000)

God. So many shared libraries. It will be a pain in the ass to run it
everywhere. But you could alonexec it.

        pcboy@home $> IFS=$'\n'; for i in `ldd ./my_binary | awk {'print $3'} | grep \/`;do cp "$i" ./;done

It will copy every shared library to the actual dir. Next we need to
create an alonexec template.

        pcboy@home $> IFS=$'\n'; for i in `ldd ./my_binary | awk {'print $1'}`;do echo \
        "($i (sourcepath $i) (destpath $i) (mode 'rwxr-xr-x')(autostart false))"\
        >> static.alonexec;done

No we have a static.alonexec full of shared libraries.

-   Warning: You \_must\_ remove some of them. Especially libdl.so.x,
    linux-gate.so.1, and /lib/ld-linux.so.2. But some others could be
    removed too (like libc). Otherwise it will not work properly on
    other systems.\* We need two more entries in our static.alonexec
    template. One for my\_binary and a script which will start it with a
    LD\_LIBRARY\_PATH="\`dirname \$0\`".

<!-- -->

        (my_binary (sourcepath my_binary) (destpath my_binary) (mode 'rwxr-xr-x')(autostart false))
        (starter.sh (sourcepath starter.sh) (destpath starter.sh) (mode 'rwxr-xr-x')(autostart true))

starter.sh:

        #!/bin/sh
        LD_LIBRARY_PATH="`dirname $0`" ./my_binary

Here we go. When you will start the binary made by alonexec, starter.sh
will be launched and my\_binary will run with all the shared libs in its
folder.

License
-------

Alonexec is under the Do What The Fuck You Want To Public License
(WTFPL).

Platforms
---------

Linux & Win32.

