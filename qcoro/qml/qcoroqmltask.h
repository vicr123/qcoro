// SPDX-FileCopyrightText: 2022 Jonah Brüchert <jbb@kaidan.im>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <QCoro/QCoroTask>
#include <QJSValue>
#include <QJSEngine>

#include "qcoroqml_export.h"

#include <type_traits>


namespace QCoro {

struct QmlTaskPrivate;

//! QML type that allows to react to asynchronous computations from QML
struct QCOROQML_EXPORT QmlTask {
    Q_GADGET

public:
    // Just for Q_DECLARE_METATYPE to be happy
    explicit QmlTask() noexcept;
    QmlTask(const QmlTask &other);
    QmlTask &operator=(const QmlTask &other);
    ~QmlTask();

    //! Constructs a QmlTask from a QCoro::Task that resolves to a QVariant
    /*!
     * \param[in] task to await
     */
    QmlTask(QCoro::Task<QVariant> &&task);

    //! Constructs a QmlTask from a QCoro::Task that resolves to an arbitrary type
    /*!
     * \param[in] task to await
     */
    template <typename T>
    QmlTask(QCoro::Task<T> &&task) : QmlTask(
        task.then([](T &&result) -> QCoro::Task<QVariant> {
            co_return QVariant::fromValue(std::forward<T>(result));
        }))
    {
        // To rely on Qt's assertion to check whether the type is a registered metatype
        qMetaTypeId<T>();
    }

    //! Constructs a QmlTask from any awaitable type supported by QCoro.
    /*!
     * \param[in] object to await
     */
    template <typename T>
    requires (detail::TaskConvertible<T> && !std::is_same_v<T, QmlTask>)
    QmlTask(T &&future) : QmlTask(detail::toTask(std::forward<T>(future)))
    {
    }

    //! Constructs a QmlTask from a QCoro::Task that doesn't return a value
    /*!
     * \param[in] task to await
     */
    template <typename T = void>
    QmlTask(QCoro::Task<> &&task) : QmlTask(
        task.then([]() -> QCoro::Task<QVariant> {
            co_return QVariant();
        }))
    {
    }

    //! QML function that executes a given JavaScript function once the computation is finished
    /*!
     * \param[in] JavaScript function to call once the result is ready.
     *  The result will be passed as first argument to the given function
     */
    Q_INVOKABLE void then(QJSValue func);

private:
    QSharedDataPointer<QmlTaskPrivate> d;
};

}

Q_DECLARE_METATYPE(QCoro::QmlTask)
