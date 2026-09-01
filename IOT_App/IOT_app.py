import requests
import json


# 1. Define your configuration parameters
CHANNEL_ID = "3475843"
READ_API_KEY = "AX1XHZR8ILSRE8MA"

# 2. Build the API URL for the last feed entry
url = f"https://api.thingspeak.com/channels/3475843/feeds.json?results=2"

# Add the API key parameters if it is a private channel
params = {"api_key": READ_API_KEY} if READ_API_KEY else {}

try:
    # 3. Send HTTP GET request to ThingSpeak
    response = requests.get(url, params=params)
    response.raise_for_status()  # Check for HTTP errors
    
    # 4. Parse JSON data
    data = response.json()
    ex_data = data['feeds'][0]

    print(ex_data)
    print(type(ex_data))
    
    # 5. Extract timestamp and fields

    timestamp =  ex_data["created_at"]#data.get("created_at")
    field1_val = ex_data["field1"]
    field2_val = ex_data["field2"]
    
    # 6. Display results
    print(f"Time: {timestamp}")
    print(f"Field 1 Value: {field1_val}")
    print(f"Field 2 Value: {field2_val}")

except requests.exceptions.RequestException as e:
    print(f"Error fetching data from ThingSpeak: {e}")
