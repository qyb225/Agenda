from django.shortcuts import render
from django.http import HttpRequest, HttpResponse, HttpResponseRedirect, JsonResponse
from basic.models import *

def index(request):
    username = request.session.get('username')
    if username:
        return HttpResponseRedirect('/meetings/')
    else:
        return HttpResponseRedirect('/login/')

def register(request):
    if request.method == "POST":
        userInfo = request.POST
        username = userInfo['username'] and (not User.objects.filter(username__exact=userInfo['username']))
        password = userInfo['password']
        email = userInfo['email']
        phone = userInfo['telephone']
        if username and password and email and phone:
            User.objects.create (
                username = userInfo['username'],
                password = passwordEncode(userInfo['password']),
                email = userInfo['email'],
                telephone = userInfo['telephone']
            )
            return render(request, 'regSuccess.html')
        else:
            return HttpResponse("fail")
    else:
        return render(request, 'register.html')

def login(request):
    if request.method == "POST":
        userInfo = request.POST
        if User.objects.filter(username__exact=userInfo['username'], password__exact=passwordEncode(userInfo['password'])):
            response = HttpResponseRedirect('/')
            request.session['username'] = userInfo['username']
            request.session.set_expiry(3600)
            return response
        else:
            return HttpResponseRedirect('/login/')
    else:
        return render(request, 'login.html')

def logout(request):
    response = HttpResponseRedirect('/login/')
    del request.session['username']
    return response

def getAllMeetings(request):
    username = request.session.get('username')
    if username:
        allMeetings = []
        allSponsorMeetings = Meeting.objects.filter(sponsor=username)
        for sponsorMeeting in allSponsorMeetings:
            allMeetings.append(sponsorMeeting)
        allparticipatorsMeetings = MeetingParticipator.objects.filter(participator__username=username)
        for participatorMeeting in allparticipatorsMeetings:
            allMeetings.append(participatorMeeting.meeting)
        return render(request, 'index.html', {'username': username, 'guide' : "My meeting", 'Meetings' : allMeetings})
    else:
        return HttpResponseRedirect('/login/')

def meetingDetails(request, meetingTitle):
    username = request.session.get('username')
    if username:
        theMeeting = Meeting.objects.get(title=meetingTitle)
        allparticipatorsMeetings = MeetingParticipator.objects.filter(meeting__title=meetingTitle)
        allparticipators = []
        for participatorMeeting in allparticipatorsMeetings:
            allparticipators.append(participatorMeeting.participator)
        if username == theMeeting.sponsor:
            return render(request, 'modifyMeeting.html', {'username': username, 'guide' : 'Meeting Details', 'meeting' : theMeeting, 'Parts' : allparticipators})
        else:
            return render(request, 'meetingDetails.html', {'username': username, 'guide' : "Meeting Details", 'meeting' : theMeeting, 'Parts' : allparticipators})
        
    else:
        return HttpResponseRedirect('/login/')

def addParticipator(request, meetingTitle, participator):
    username = request.session.get('username')
    if username:
        theMeeting = Meeting.objects.filter(title=meetingTitle)
        if len(theMeeting) == 0 or theMeeting[0].sponsor != username or username == participator \
         or MeetingParticipator.objects.filter(meeting__title=meetingTitle, participator__username=participator):
            return HttpResponse("error")
        thePart = User.objects.filter(username=participator)
        if theMeeting and thePart:
            MeetingParticipator.objects.create (
                meeting = theMeeting[0],
                participator = thePart[0]
            )
            return JsonResponse({'username': thePart[0].username, 'telephone': thePart[0].telephone})
        return HttpResponse("error")
    else:
        return HttpResponseRedirect('/login/')

def createMeeting(request):
    username = request.session.get('username')
    if username:
        if request.method == 'POST':
            meetingInfo = request.POST
            Meeting.objects.create (
                sponsor = username,
                startDate = meetingInfo['StartTime'],
                endDate = meetingInfo['EndTime'],
                title = meetingInfo['title']
            )
            return HttpResponseRedirect('/meetings/')
        else:
            return render(request, 'create.html', {'username': username, 'guide' : "Create Meeting"})
    else:
        return HttpResponseRedirect('/login/')

def getAllUsers(request):
    username = request.session.get('username')
    if username:
        allUsers = User.objects.all()
        total = len(allUsers) // 9
        return render(request, 'users.html', {'username': username, 'guide' : "All Users", 'total' : range(1, total + 2)})
    else:
        return HttpResponseRedirect('/login/')

def getPageUsers(request, numOf):
    username = request.session.get('username')
    if username:
        li = 9 * (int(numOf) - 1)
        allUsers = User.objects.all()
        ma = len(allUsers)
        pageUsers = []
        for i in range(li, li + 8):
            if i >= ma:
                break
            pageUsers.append(allUsers[i])
        return render(request, 'userlist.html', {'Users' : pageUsers})
    else:
        return HttpResponse("")

def meetingDelete(request, meetingTitle):
    username = request.session.get('username')
    if username:
        theMeeting = Meeting.objects.filter(title=meetingTitle)
        if theMeeting:
            targetMeeting = theMeeting[0]
            if targetMeeting.sponsor == username:
                theMeeting.delete()
        return HttpResponseRedirect('/')
    else:
        return HttpResponseRedirect('/login/')

def searchUser(request, username):
    if User.objects.filter(username__exact=username):
        return HttpResponse(username)
    return HttpResponse("error")

def searchMeeting(request, meetingTitle):
    if Meeting.objects.filter(title=meetingTitle):
        return HttpResponse(meetingTitle)
    return HttpResponse("error")

def passwordEncode(str):
    import hashlib
    m = hashlib.md5(str.encode(encoding='utf-8'))
    return m.hexdigest()