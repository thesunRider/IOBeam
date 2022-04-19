from django.shortcuts import render,redirect
from django.http import HttpResponse
from django.contrib.auth.decorators import login_required
from django.http import JsonResponse
from django.contrib.auth import login
from django.views.decorators.csrf import csrf_exempt
import uuid,json



from home.models import device

@login_required
def index(request):
	device_list = list(device.objects.filter(device_owner__email__contains=request.user.email).values())
	context = {'device_list':device_list }
	return render(request, "dashboard.template",context=context)

#def bad_request(request):
#	return render(request, "400.template")

#def permission_denied(request):
#	return render(request, "403.template")

def page_not_found(request,exception, template_name="404.html"):
	return render(request, "404.template")

def server_error(request):
	return render(request, "500.template")

@login_required
def delete_model(request):
	if request.method == 'POST' and 'device_uid' in request.POST:
		 device.objects.filter(device_uid__contains=request.POST['device_uid']).delete()

	return redirect('dashboard')




