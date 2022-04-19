from django.http import JsonResponse
from django.contrib.auth import authenticate



#support functions for connector-home integrations on home

def connector_checkuser(request):
	if 'device_type' in request.POST and 'device_version' in request.POST and 'device_uid' in request.POST:
		if request.method == 'POST':
			email = request.POST["email"]
			password = request.POST["password"]

			user = authenticate(
					request, 
					username=email, 
					password=password
			)

			if user is None:			
				return False

			else:
				print("Device registering")
				return user

	return False