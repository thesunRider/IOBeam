"""iobeam_website URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/4.0/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import include, path
from django.conf.urls import (
handler400, handler403, handler404, handler500
)

#handler400 = 'home.views.bad_request'
#handler403 = 'home.views.permission_denied'
handler404 = 'home.views.page_not_found'
handler500 = 'home.views.server_error'


urlpatterns = [
	path('', include('home.urls')),
	path('dashboard/', include('home.urls')),
	path('site-admin/', admin.site.urls),
	path('auth/', include('authentication.urls')),
	path('polls/', include('polls.urls')),
	path('device_show/',include('device_show.urls'))
]
