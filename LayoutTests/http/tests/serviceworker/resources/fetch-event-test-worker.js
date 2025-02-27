function handleString(event) {
    event.respondWith(new Response('Test string'));
}

function handleBlob(event) {
    event.respondWith(new Response(new Blob(['Test blob'])));
}

function handleReferrer(event) {
    event.respondWith(new Response(new Blob(['Referrer: ' + event.request.referrer])));
}

function handleNullBody(event) {
    event.respondWith(new Response(null));
}

function handleReject(event) {
    event.respondWith(new Promise(function(resolve, reject) {
        reject('rejected!');
    }));
}

function handleFetch(event) {
    event.respondWith(fetch('other.html'));
}

self.addEventListener('fetch', function(event) {
    var url = event.request.url;
    var handlers = [
        { pattern: '?string', fn: handleString },
        { pattern: '?blob', fn: handleBlob },
        { pattern: '?referrer', fn: handleReferrer },
        { pattern: '?ignore', fn: function() {} },
        { pattern: '?null', fn: handleNullBody },
        { pattern: '?reject', fn: handleReject },
        { pattern: '?fetch', fn: handleFetch }
    ];

    var handler = null;
    for (var i = 0; i < handlers.length; ++i) {
       if (url.indexOf(handlers[i].pattern) != -1) {
           handler = handlers[i];
           break;
       }
    }

    if (handler)
        handler.fn(event);
    else
        event.respondWith(new Response(new Blob(['Service Worker got an unexpected request: ' + url])));
});
