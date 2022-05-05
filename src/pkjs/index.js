// слушает на наличие события "Первый запуск"
Pebble.addEventListener('ready', function(e) {
    console.log('PebbleKit JS ready!');
});

// слушает на наличие сообщений с телефона
Pebble.addEventListener('appmessage', function(e) {
    console.log('AppMessage recieved!');
});