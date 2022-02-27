from django.http import JsonResponse
from django.contrib.auth import authenticate

#support functions for connector-home integrations on home

def connector_checkuser(request):
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
			return True