from django.shortcuts import render
from polls.additionals import *

from django.http import HttpResponse
from django.contrib.auth.decorators import login_required
from django.http import JsonResponse
from django.contrib.auth import login
from django.views.decorators.csrf import csrf_exempt
import uuid,json


from home.models import device


# Create your views here.


#----------------------------------------CONNECTOR_INTERFACE-----------------------------
#	201 - Invalid credentials
#	202	- Un-Registered Devices
#	400 - Device registered
#	202 - Re-register Error
#	401 - List devices
#	402 - Login Success
#
#

#call with devuce_uid as parameter,email and parameter
@csrf_exempt
def connector_registerdevice(request):
	user = polldevice_checkuser(request)
	if user:
		if len(device.objects.filter(device_uid__contains=request.POST['device_uid'])) > 0:
			return JsonResponse({"msg":"Re-register Error","code":202,"dev_code":0})

		device_in = device(device_type=request.POST['device_type'], device_version=request.POST['device_version'],device_owner=user,device_status=True,device_uid = request.POST['device_uid'])
		device_in.save()
		login(request, user)
		return JsonResponse({"msg":"Device Registered","code":400,"dev_code":device_in.device_uid})
	else:
		return JsonResponse({"msg":"Invalid credentials","code":201,"dev_code":0})

#just call for devices for the user
@login_required
def connector_listdevices(request):
	device_list = list(device.objects.filter(device_owner__email__contains=request.user.email).values())
	return JsonResponse({"msg":"success","code":401,"dev_code":device_list})



@csrf_exempt
@login_required
def get_status(request):
	device_in = device.objects.filter(device_uid__contains=request.GET.get('device'))
	if request.method == 'GET' and device_in:
		return JsonResponse(list(device_in.values())[0])

#call with email and parameter
@csrf_exempt
def login_device(request):
	user = polldevice_checkuser(request)
	if user:
		if len(device.objects.filter(device_uid__contains=request.POST['device_uid'])) > 0:
			login(request, user)
			return JsonResponse({"msg":"LOGIN success","code":402,"dev_code":0})
		else:
			return JsonResponse({"msg":"Un-Registered Device","code":202,"dev_code":0})
	
	else:
		return JsonResponse({"msg":"Invalid credentials","code":201,"dev_code":0})

