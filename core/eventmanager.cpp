#include "eventmanager.hpp"

#include <QWebEngineView>

#include "ipcclient.hpp"

EventManager::EventManager(QObject *parent)
    : QObject(parent)
{
}

void EventManager::bind()
{
    // url_changed EVENT
    connect(m_webView, &QWebEngineView::urlChanged, [=](const QUrl &url) {
        foreach (const Event &event, m_subscribers["url_changed"]) {
            if (event.subscriptionCommand().isSingleShot()) {
                event.subscriptionCommand().sendResponse(QString("%1").arg(url.toString()).toUtf8());
            } else {
                event.subscriptionCommand().sendResponse(QString("url_changed %1").arg(url.toString()).toUtf8());
            }
        }
    });

    // title_changed EVENT
    connect(m_webView, &QWebEngineView::titleChanged, [=](const QString &title) {
        foreach (const Event &event, m_subscribers["title_changed"]) {
            if (event.subscriptionCommand().isSingleShot()) {
                event.subscriptionCommand().sendResponse(QString("%1").arg(title).toUtf8());
            } else {
                event.subscriptionCommand().sendResponse(QString("title_changed %1").arg(title).toUtf8());
            }
        }
    });

    // load_started EVENT
    connect(m_webView, &QWebEngineView::loadStarted, [=]() {
        foreach (const Event &event, m_subscribers["load_started"]) {
            if (event.subscriptionCommand().isSingleShot()) {
                event.subscriptionCommand().sendResponse(QString("").toUtf8());
            } else {
                event.subscriptionCommand().sendResponse(QString("load_started").toUtf8());
            }
        }
    });

    // load_finished EVENT
    connect(m_webView, &QWebEngineView::loadFinished, [=](bool ok) {
        foreach (const Event &event, m_subscribers["load_finished"]) {
            if (event.subscriptionCommand().isSingleShot()) {
                event.subscriptionCommand().sendResponse(QString("%1").arg(ok).toUtf8());
            } else {
                event.subscriptionCommand().sendResponse(QString("load_finished %1").arg(ok).toUtf8());
            }
        }
    });
}

void EventManager::subscribe(const Event &event)
{
    m_subscribers[event.name()].append(event);

    // Unsubscribe
    connect(event.subscriptionCommand().client(), &QObject::destroyed, [this, event]() {
        m_subscribers[event.name()].removeOne(event);
    });
}

QWebEngineView *EventManager::webView()
{
    return m_webView;
}

void EventManager::setWebView(QWebEngineView *webView)
{
    m_webView = webView;
}
