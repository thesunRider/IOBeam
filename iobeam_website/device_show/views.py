from django.shortcuts import render,redirect
from django.contrib.auth.decorators import login_required

from home.models import device
from .forms import EditorForm

# Create your views here.
#1630 - Beam Steerer
#1678 - Beam Chopper


@login_required
def device_show(request):
	device_list = list(device.objects.filter(device_owner__email__contains=request.user.email ,).values())
	context = {'device_list':device_list}

	device_in = device.objects.filter(device_uid__contains=request.GET.get('device'))
	if request.method == 'GET' and device_in:

		if device_in[0].device_type == 1630:
			context['device_in'] = list(device_in.values())[0]
			context['title_page'] = "Beam Steering"
			print(context)
			form=EditorForm()
			return render(request,"beamsteering.template",{"context":context,"form":form})


	return redirect('dashboard')