// словарь данных о погоде
var dictionary = {
    "TEMPERATURE": temperature,
    "CONDITIONS": conditions
}

// отправка сообщении о получении / не получении данных о погоде
Pebble.sendAppMessage(dictionary, function(e) {
    console.log('Weather info sent to Pebble successfully!');
},
function(e) {
    console.log('Error sending weather info to Pebble!');
}
)

// отправка запроса
var xhrRequest = function(url, type, callback) {
    var xhr = new XMLHttpRequest();
    xhr.onload = function() {
        callback(this.responseText);
    };
    xhr.open(type, url);
    xhr.send();
}

// успешное определение геолокации. отправляет запрос openweather
function locationSuccess(pos) {
    var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude + '&appid=' + myAPIKey;

    xhrRequest(url, 'GET', function(responseText) {
        // парсим json
        var json = JSON.parse(responseText);

        // температура
        var temperature = Math.round(json.main.temp - 273.15);
        console.log('Temperature is ' + temperature);

        // погодные условия
        var conditions = json.weather[0].main;
        console.log('Conditions are ' + conditions);
    })
    //somecode
}

// ошибка определения геолокации
function locationError(err) {
    console.log('Error requesting location!');
}

// получение информации о погоде
function getWeather() {
    navigator.geolocation.getCurrentPosition(
        locationSuccess,
        locationError,
        {timeout: 15000, maximumAge: 60000}
    );
}

// слушает на наличие запуска циферблата
Pebble.addEventListener('ready', function(e) {
    console.log('PebbleKit JS ready!');

    getWeather();
});

// слушает на наличие сообщений с телефона
Pebble.addEventListener('appmessage', function(e) {
    console.log('AppMessage recieved!');
    getWeather();
});

