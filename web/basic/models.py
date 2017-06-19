from django.db import models

class User(models.Model):
    username = models.CharField(max_length=50)
    password = models.CharField(max_length=50)
    email = models.CharField(max_length=50)
    telephone = models.CharField(max_length=50)

    def __str__(self):
        return self.username

class Meeting(models.Model):
    sponsor = models.CharField(max_length=50)
    startDate = models.CharField(max_length=20)
    endDate = models.CharField(max_length=20)
    title = models.CharField(max_length=50)

    def __str__(self):
        return self.title

class MeetingParticipator(models.Model):
    meeting = models.ForeignKey(Meeting)
    participator = models.ForeignKey(User)