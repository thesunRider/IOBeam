from django.db import models
from django.contrib.auth.models import User
import uuid

# Create your models here.
#---------DEVICE UID---------
#1630 - Beam Steerer 
#1678 - Beam Chopper


class device(models.Model): # Product is the name of our model
	device_id = models.AutoField
	device_uid = models.CharField(	
			max_length=100,
			default="0",
			unique=True,
			error_messages ={
			"unique":"You cant-re register the same device."
		
		})
	
	device_type = models.IntegerField()
	device_version = models.CharField(max_length=100)
	device_owner = models.ForeignKey(User,on_delete=models.CASCADE,default=None)
	device_status = models.BooleanField()
	device_data = models.CharField(max_length=2000,default="0")

	def get_devicename():
		if self.device_type == 1630:
			return "beam_steerer_v1"
		elif self.device_type == 1678:
			return "beam_chopper_v1"

	def get_deviceurl():
		return  device_uid &"/"

	def device_connected():
		#check if device is connected or not 
		return self.device_status
