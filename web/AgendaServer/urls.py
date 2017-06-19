"""AgendaServer URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.11/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url
from django.contrib import admin
from basic import views as views

urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^$', views.index, name='index'),
    url(r'^register/(?P<username>\w+)/*$', views.searchUser, name='searchUser'),
    url(r'^register/*$', views.register, name='register'),
    url(r'^login/*$', views.login, name='login'),
    url(r'^logout/*$', views.logout, name='logout'),
    url(r'^meetings/*$', views.getAllMeetings, name='meetings'),
    url(r'^meetings/(?P<meetingTitle>\w+)/*$', views.meetingDetails, name='meetingDetails'),
    url(r'^meetings/(?P<meetingTitle>\w+)/add/(?P<participator>\w+)/*$', views.addParticipator, name='addParticipator'),
    url(r'^meetings/(?P<meetingTitle>\w+)/delete/*$', views.meetingDelete, name='meetingDelete'),
    url(r'^create/*$', views.createMeeting, name='createmeeting'),
    url(r'^create/search/(?P<meetingTitle>\w+)/*$', views.searchMeeting, name='searchMeeting'),
    url(r'^users/*$', views.getAllUsers, name='allUsers'),
    url(r'^users/(?P<numOf>\d+)/*$', views.getPageUsers, name='pageUsers'),
]
