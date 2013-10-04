/**
@file observable.c File that provides implementations of functions for observable pattern
 */
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

#include "observable.h"

/******************** OBSERVABLE IMPLEMENTATION ********************/

/** 
 * @author Ivan Pusic
 * @brief Function to registering new observer. Observer is added to list of all observers
 * @param self Observable object instance
 * @param observer Observer object instance
 * 
 * @return Exit status 
 */
static inline int AK_register_observer(AK_observable *self, AK_observer *observer)
{
    int i;
    for (i = 0; i < MAX_OBSERVABLE_OBSERVERS; ++i) {
        if(self->observers[i] == NULL) {
            // Assigning unique ID to new observer
            observer->observer_id = self->observer_id_counter++;
            self->observers[i] = observer;
            return OK;
        }
    }
    
    return NOT_OK;
}

/** 
 * @author Ivan Pusic
 * @brief Function to unregister observer. Observer will be removed from list of all observers
 * @param self Observable object instance
 * @param observer Observer object instance
 * 
 * @return Exit status
 */
static inline int AK_unregister_observer(AK_observable *self, AK_observer *observer)
{
    int i;
    for(i = 0; i < MAX_OBSERVABLE_OBSERVERS; ++i) {
        if(observer == self->observers[i]) {
            free(self->observers[i]);
            self->observers[i] = NULL;
            return OK;
        }
    }

    return NOT_OK;
}

/** 
 * @author Ivan Pusic
 * @brief Function to notiy certain observer
 * @param self Observable object instance
 * @param observer Observer object to notify
 * 
 * @return Exit status
 */
static inline int AK_notify_observer(AK_observable *self, AK_observer *observer)
{
    int i;
    for(i = 0; i < MAX_OBSERVABLE_OBSERVERS; ++i) {
        if(self->observers[i] != NULL && self->observers[i] == observer) {
            observer->AK_notify(observer, self->AK_observable_type, self->AK_ObservableType_Def);
            return OK;
        }
    }
    
    return NOT_OK;
}

/** 
 * @author Ivan Pusic
 * @brief Function to notify all observers
 * @param self Observable object instance
 * 
 * @return Exit status
 */
static inline int AK_notify_observers(AK_observable *self)
{
    int i;
    for(i = 0; i < MAX_OBSERVABLE_OBSERVERS; ++i) {
        if(self->observers[i] != NULL) {
            // Call AK_notify and pass AK_observer observer and custom observable instances
            self->observers[i]->AK_notify(self->observers[i], self->AK_observable_type, self->AK_ObservableType_Def);
        }
    }
    
    return OK;
}

/** 
 * @author Ivan Pusic
 * @brief Function for getting observer object from observable type by observer id
 * @param self Observable type instance
 * @param id Observer id
 * 
 * @return Requested observer instance
 */
static inline AK_observer *AK_get_observer_by_id(AK_observable *self, int id)
{
    int i;
    for(i = 0; i < MAX_OBSERVABLE_OBSERVERS; ++i) {
        if(self->observers[i] != NULL && self->observers[i]->observer_id == id) {
            return self->observers[i];
        }
    }
    return NULL;
}

/** 
 * @author Ivan Pusic
 * @brief Function for initializing observable object
 *
 * @return Pointer to new observable object
 */
AK_observable * AK_init_observable(void *AK_observable_type, AK_ObservableType_Enum AK_ObservableType_Def, void * AK_custom_action)
{
    AK_observable *self;
    self = (AK_observable*) calloc(1, sizeof(*self));
    self->AK_register_observer = &AK_register_observer;
    self->AK_unregister_observer = &AK_unregister_observer;
    self->AK_notify_observer = &AK_notify_observer;
    self->AK_notify_observers = &AK_notify_observers;
    self->AK_get_observer_by_id = AK_get_observer_by_id;
    self->AK_run_custom_action = AK_custom_action;

    memset(self->observers, 0, sizeof self->observers);
           
    
    self->AK_ObservableType_Def = AK_ObservableType_Def;
    self->observer_id_counter = 1;
    self->AK_observable_type = AK_observable_type;
    return self;
}

/******************** OBSERVER IMPLEMENTATION ********************/

/** 
 * @author Ivan Pusic
 * @brief Function for destroying observer object
 *
 * @return Exit status
 */
static inline int AK_destroy_observer(AK_observer *self)
{
    if(self != NULL) {
        free(self);
        self = NULL;   
        return OK;
    }
    
    return NOT_OK;
}

/** 
 * @author Ivan Pusic
 * @brief Function for calling event handler of observer object
 * @param observer AK_observer type instance
 * @param observable_type Custom observable type instance
 * 
 * @return Exit status
 */
static inline int AK_notify(AK_observer *observer, void *observable_type, AK_ObservableType_Enum type)
{
    observer->AK_observer_type_event_handler(observer->AK_observer_type, observable_type, type);
    return OK;
}

/** 
 * @author Ivan Pusic
 * @brief Function for initializing observer object
 * 
 * @return Pointer to new observer object
 */
AK_observer *AK_init_observer(void *observer_type, void (*observer_type_event_handler)(void*, void*, AK_ObservableType_Enum))
{
    AK_observer *self;
    self = calloc(1, sizeof(*self));
    self->AK_destroy_observer = &AK_destroy_observer;
    self->AK_observer_type = observer_type;
    self->AK_observer_type_event_handler = observer_type_event_handler;
    self->AK_notify = &AK_notify;
    return self;
}
