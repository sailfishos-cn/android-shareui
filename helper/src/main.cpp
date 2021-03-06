#include <QFile>
#include <QTextStream>

#include <sys/types.h>
#include <unistd.h>

#include <QCoreApplication>
#include "dbusmain.h"
#include <QTimer>

#include <QDebug>

int main(int argc, char *argv[])
{
    ::setgid(0);
    ::setuid(0);
    ::chroot("/opt/alien");
    ::chdir("/");

    // Hacka hacking hacky-hacky hacked hacku hacka hack.
    qputenv("DBUS_SESSION_BUS_ADDRESS", "unix:path=/run/user/100000/dbus/user_bus_socket");

    qputenv("LD_LIBRARY_PATH", "/system/vendor/lib:/system/lib:/vendor/lib:/system_jolla/lib:");
    qputenv("SYSTEM_USER_LANG", "C");

    qputenv("ANDROID_ROOT", "/system");
    qputenv("ANDROID_DATA", "/data");

    qputenv("CLASSPATH", "/system/framework/am.jar");

    QFile init("/system/script/start_alien.sh");
    if (init.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&init);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith("export BOOTCLASSPATH=")) {
                line=line.mid(21).replace("$FRAMEWORK", "/system/framework");
                qputenv("BOOTCLASSPATH", line.toUtf8());
                break;
            }
        }
    }
    else {
        return 0;
    }

    QFile envsetup("/system/script/platform_envsetup.sh");
    if (envsetup.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&envsetup);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith("export ALIEN_ID=")) {
                line=line.mid(16);
                qputenv("ALIEN_ID", line.toUtf8());
                break;
            }
        }
    }
    else {
        return 0;
    }

    QCoreApplication app(argc, argv);
    DBusMain *bus = new DBusMain();
    QTimer::singleShot(0, bus, SLOT(start()));
    return app.exec();
}
