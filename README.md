# Fire-Animation
A fire Ascii animation for The game The Infernal Canyon

##Useful struct :
struct Fire
###Useful values in it :
intensity : Numbers of case randomly burned
cooling : Numbers of case randomly cooled down to 0
max\_fire : maximum value given randomly when update\_fire_logic()


##Useful functions :
fire_create() -to get a struct fire * object
fire_destroy() -to destroy a struct fire * object (malloc type shi)

get\_char_fire() -to get the visual representation of this heat in the palette
update\_fire_logic() -to update the state of fire (call at a regular interval of time)

