# Kaya_Office_Hours

So for this mini project Prof. Kaya wanted to be able to display messages in his office. If he was sick, changed his office hours or just wanted to write a nice message for anyone passing by, this LED panel display his message. 


One of the biggest hurdles was figuring out how to remotely access the display. I did not want to do anything too difficult so that in the future if Prof. Kaya decided to change his host site or the colors of the display, he could do so with ease. I thought of using a Bluetooth module or a Wifi module. However, these modules could only be accessed if the Wifi is the same or if his phone is close to the display. 

I used 000webhost in order to host the site. The site just uses a simple index.php file to allow the user to input a message. 

This is what the site looks like: 
<img width="1435" alt="Screenshot 2024-06-26 at 12 11 48 PM" src="https://github.com/liapia99/Kaya_Office_Hours/assets/98356859/21a2b9ec-658e-4008-8430-443cd7015914">

For the display, I used two 32 by 8 LED panels, a RTC and a Raspberry Pico W. 

This is what it looks like:
![IMG_1044](https://github.com/liapia99/Kaya_Office_Hours/assets/98356859/83f204bd-a07c-4a6b-8d23-31f24550e15c)

Sources used: 
- https://www.w3schools.com/php/php_forms.asp
- https://www.w3.org/International/articles/http-charset/index
- https://www.000webhost.com/
- https://www.youtube.com/watch?v=WiW9Bt38qUM&ab_channel=Mario%27sIdeas
- https://www.youtube.com/watch?v=gBofy7MMdIY&ab_channel=educ8s.tv
