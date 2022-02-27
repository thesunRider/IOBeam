from django.shortcuts import render
from django.http import HttpResponse
from django.contrib.auth.decorators import login_required
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt



from home.additionals import *

@login_required
def index(request):
	return render(request, "dashboard.template")

#def bad_request(request):
#	return render(request, "400.template")

#def permission_denied(request):
#	return render(request, "403.template")

def page_not_found(request,exception, template_name="404.html"):
	return render(request, "404.template")

def server_error(request):
	return render(request, "500.template")



#----------------------------------------CONNECTOR_INTERFACE-----------------------------
#	201 - Invalid credentials
#	400 - Device registered
#
#
#
#

@csrf_exempt
def connector_registerdevice(request):
	if connector_checkuser(request):
		return JsonResponse({'msg':'Device Registered','code':400})
	else:
		return JsonResponse({'msg':'Invalid credentials','code':201})




