from django.http import JsonResponse
from django.contrib.auth.models import User
from django.db.models import Q 

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
			users = list(User.objects.filter(first_name__icontains=user_uid ))
			print(users,user_uid)
			if(users and len(users)>0):
				user = users[0]

				if user is None:			
					return False

				else:
					print("Device registering",user)
					return user

	return False