alert_body = 'MATLAB Thingspeak';
alert_subject = 'Status changed';
alert_api_key = 'TAK4PNJF11EPYRYXUHPE8';
alert_url = "https://api.thingspeak.com/alerts/send";

channelID = 2349977; % channel ID
readAPIKey = 'GOOTZORSIYFDUPP9'; % Read API Key 

% Read the latest values from Fields 4, 5, 6, and 7
fieldValues = thingSpeakRead(channelID, 'Fields', [4, 5, 6, 7], 'ReadKey', readAPIKey);


% New: Include the field value
alert_body = sprintf('AC: %d, Heater : %d, Blind: %d, RHED: %d', fieldValues);

% Form the JSON message
jsonmessage = sprintf(['{"subject": "%s", "body": "%s"}'], alert_subject, alert_body);

% Set options for web request
options = weboptions("HeaderFields", {'Thingspeak-Alerts-API-Key', alert_api_key; 'Content-Type','application/json'});

% Send the request
result = webwrite(alert_url, jsonmessage, options);
