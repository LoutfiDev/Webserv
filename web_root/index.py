import requests

# URL to send the GET request to
url = 'http://localhost:4342'

# Headers to include in the GET request
headers = {
	'Status': '301',
    'Location': 'http://google.com'
}

# Send the GET request
response = requests.get(url, headers=headers, allow_redirects=False)

# Print the final URL after redirection (if any)
# print(response.status_code)

# Print the response content
# print(response.url)
