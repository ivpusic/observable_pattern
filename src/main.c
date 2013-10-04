/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "observable.h"

/******************** OBSERVABLE PATTERN EXAMPLE ********************/

// This is optional. You can define some notifyTypes for your custom observable type
typedef enum {
    ERROR,
    INFO,
    WARMING,
} NotifyType;

// This is also optional. Here we define structure that holds message, and notiy type
typedef struct _notifyDetails {
    char *message;
    NotifyType type;
} NotifyDetails;

// First define you observable type 
struct TypeObservable {
    // Then specify your type variables
    NotifyDetails *notifyDetails;
    // And your methods
    char* (*AK_get_message) (struct TypeObservable*);
    
    // You should define methods for adding and removing observers. You can also define some other method for achieveing this
    int (*AK_custom_register_observer) (struct TypeObservable*, AK_observer*);
    int (*AK_custom_unregister_observer) (struct TypeObservable*, AK_observer*);
    // If you want you can define method for setting observable type and message for our custom observable type
    void (*AK_set_notify_info_details) (struct TypeObservable*, NotifyType type, char *message);
    // Define observable variable, nedded for observable pattern. Important!
    AK_observable *observable;
};
typedef struct TypeObservable AK_TypeObservable;

// Implement method for getting some message from observable custom type
char * AK_get_message(AK_TypeObservable *self) {
    return self->notifyDetails->message;
}

// Implement method for adding observer to custom observable type
int AK_custom_register_observer(AK_TypeObservable* self, AK_observer* observer) {
    self->observable->AK_register_observer(self->observable, observer);
    return OK;
}

// Implement method for removing observer from custom observable type
int AK_custom_unregister_observer(AK_TypeObservable * self, AK_observer* observer) {
	self->observable->AK_unregister_observer(self->observable, observer);
    return OK;
}

// Helper method for setting notify details
void AK_set_notify_info_details(AK_TypeObservable *self, NotifyType type, char *message) {
    // Info about notify
    NotifyDetails *notifyDetails;
    notifyDetails = calloc(1, sizeof(NotifyDetails));
    notifyDetails->message = message;
    notifyDetails->type = type;
    self->notifyDetails = notifyDetails;
}

// this is definition of custom action which can be called from custom observable type
int AK_custom_action(void *data) {
    return OK;
}

// You should have some kind of method for initializing observable type
AK_TypeObservable * init_observable_type() {
    AK_TypeObservable *self;
    self = calloc(1, sizeof(AK_TypeObservable));
    self->AK_get_message = AK_get_message;
    self->AK_custom_register_observer = &AK_custom_register_observer;
    self->AK_custom_unregister_observer = &AK_custom_unregister_observer;
    self->AK_set_notify_info_details = &AK_set_notify_info_details;

    // Very important!!! Call method for initializing AK_Observable and pass instance of custom observable type
    // Last parameter is pointer to function which is custom
    self->observable = AK_init_observable(self, AK_CUSTOM_FIRST, &AK_custom_action);
    return self;
}

struct TypeObserver {
    // This is observable type we will track.
    // You can declare instance of custom observable type here if you want, but it isn't necessary
    AK_TypeObservable *observable;
    
    // Nedded for observable pattern. Important!
    AK_observer *observer;
}; 
typedef struct TypeObserver AK_TypeObserver;
typedef struct TypeObserver AK_TypeObserver_Second;

// Define event handler for our AK_TypeObservable type. You should also define event handlers for other types if you need.
void handle_ak_custom_type(AK_TypeObserver *observer, AK_TypeObservable *observable) {
    char *message;
    switch(observable->notifyDetails->type) {
        // we define possible notify types 
    case ERROR:
        // for every possibility some action
        message = observable->AK_get_message(observable);
        printf ("%s\n", message);
        break;
    case WARMING:
        message = observable->AK_get_message(observable);
        printf ("%s\n", message);
        break;
    case INFO:
        break;
    }
}

// This method will be called by observable type 
void custom_observer_event_handler(void *observer, void *observable, AK_ObservableType_Enum AK_ObservableType_Def) {
    // Handle event, and call some method from observable type. In this case that is method for getting some message from observable type.
    // You can also define your custom methods and call that method in event handler (here).
    switch(AK_ObservableType_Def) {
        // we define possible observable types
    case AK_TRANSACTION:
        break;
    case AK_TRIGGER:
        break;
    case AK_CUSTOM_FIRST:
        // for some type we call specified function
        handle_ak_custom_type((AK_TypeObserver*)observer, (AK_TypeObservable*)observable);
        break;
    case AK_CUSTOM_SECOND:
        break;
    }
}

// Define some method for init observer type
AK_TypeObserver * init_observer_type(void *observable) {
    AK_TypeObserver *self;
    self = calloc(1, sizeof(AK_TypeObserver));
    self->observable = observable;
    // Init AK_Observer type. This is very important!!! Pass custom type observer instance as first parameter, and
    // pointer to event handler function of custom observer type
    self->observer = AK_init_observer(self, &custom_observer_event_handler);
    return self;
}

// Define some method for init observer type. This method is without passing custom observable type to observer.
// This is also correct, if you don't want to have instance of custom observable type in custom observer type
AK_TypeObserver * init_observer_type_second() {
    AK_TypeObserver_Second *self;
    self = calloc(1, sizeof(AK_TypeObserver_Second));
    // Init AK_Observer type. This is very important!!! Pass custom type observer instance as first parameter, and
    // pointer to event handler function of custom observer type
    self->observer = AK_init_observer(self, custom_observer_event_handler);
    return self;
}

/**
 * @author Ivan Pusic
 * @brief Function that runs tests for observable pattern
 */
void AK_observable_test()
{
    printf ("\n========== OBSERVABLE PATTERN BEGIN ==========\n");

    // Fucntion for init custom observable type
    AK_TypeObservable *observable_type = init_observable_type();
    // Init observer type with passing observable type instance
    AK_TypeObserver *observer_first = init_observer_type(observable_type);
    // You can also initialize second obsever type without passing observable type instance
    AK_TypeObserver_Second *observer_second = init_observer_type_second();

    // Register out observers to observable type
    observable_type->AK_custom_register_observer(observable_type, observer_first->observer);
    observable_type->AK_custom_register_observer(observable_type, observer_second->observer);

    // Set notify type and message
    observable_type->AK_set_notify_info_details(observable_type, ERROR, "THIS IS SOME ERROR FOR ALL OBSERVERS");
    // Notify all observers
    observable_type->observable->AK_notify_observers(observable_type->observable);

    // Set notify type and message
    observable_type->AK_set_notify_info_details(observable_type, WARMING, "THIS IS SOME WARMING FOR SPECIFIED OBSERVER");
    // Notify specified observer
    observable_type->observable->AK_notify_observer(observable_type->observable, observer_first->observer);

    observable_type->observable->AK_run_custom_action(NULL);
    // Search for observer by ID
    AK_observer *requested_observer = observable_type->observable->AK_get_observer_by_id(observable_type->observable, 1);
    if(requested_observer) {
        printf ("Observer was found. Observer adress: %p\n", requested_observer);
    }
    else
        printf ("Requested observer was not found!\n");
    
    printf ("========== OBSERVABLE PATTERN END ==========\n");
}

int main(int argc, char * argv[])
{
    AK_observable_test();
    exit(EXIT_SUCCESS);
}

