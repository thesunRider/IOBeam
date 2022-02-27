from django.db import models
from django.contrib.auth.models import User
import uuid

# Create your models here.
#1630 - Beam Steerer
#1678 - Beam Chopper


class device(models.Model): # Product is the name of our model
	device_id = models.AutoField
	device_uid = str(uuid.uuid4()).replace("-","")
	device_type = models.IntegerField()
	device_version = models.CharField(max_length=100)
	device_owner = models.ForeignKey(User,on_delete=models.CASCADE)
	device_status = models.BooleanField()

	def get_devicename():
		if self.device_type == 1630:
			return "beam_steerer_v1"
		elif self.device_type == 1678:
			return "beam_chopper_v1"

	def get_deviceurl():
		return  device_uid &"/"

	def device_status():
		#check if device is connected or not 
		return True



