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



def polldevice_checkuser(request):
	if 'device_type' in request.POST and 'device_version' in request.POST and 'device_uid' in request.POST:
		if request.method == 'POST':
			user_uid = request.POST["user_uid"]
			users = User.objects.filter(first_name__icontains=user_uid )
			if(users && len(users)>0):
				user = authenticate(
						request, 
						username=users[0].email, 
						password=users[0].password
				)

				if user is None:			
					return False

				else:
					print("Device registering")
					return user

	return False