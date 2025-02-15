<!--
SPDX-FileCopyrightText: 2022 Jonah Brüchert <jbb@kaidan.im>
SPDX-License-Identifier: GFDL-1.3-or-later
-->

# QmlTask

{{ doctable("Qml", "QCoroQmlTask") }}

QmlTask allows to return [QCoro::Task][qcoro-task]s directly to QML.
It can be constructed from any [QCoro::Task][qcoro-task] that returns a value that can be converted to a [QVariant][qdoc-qml].

```C++
#include <QCoroQml>
#include <QCoroQmlTask>

int main()
{
    ...
    qmlRegisterType<Example>("io.me.qmlmodule", 1, 0, "Example");
    QCoro::Qml::registerTypes();
    ...
}

class Example : public QObject
{
    Q_OBJECT

    ...

public:
    Q_INVOKABLE QCoro::QmlTask fetchValue(const QString &name) const
    {
        return database->fetchValue(name);
        // Returns QCoro::Task<QString>
    }
}
```

```QML
Example {
    Component.onCompleted: {
        fetchValue("key").then((result) => console.log("Result", result))
    }
}
```


[qdoc-qml]: https://doc.qt.io/qt-5/qvariant.html
[qcoro-task]: ../coro/task.md
