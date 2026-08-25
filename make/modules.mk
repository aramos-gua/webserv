# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    modules.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/25 22:52:00 by emflynn           #+#    #+#              #
#    Updated: 2026/08/25 23:33:15 by emflynn          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# TODO: review comments below

# The module dependency graph, and the only place it is written down.
#
# A Makefile sets NAME, then includes this file. MODULES comes back holding the
# full transitive closure of that module's dependencies, ordered so that every
# module precedes the modules it depends on.
#
# That ordering is what the link line needs: static archives carry no dependency
# metadata, and GNU ld makes a single pass over them, pulling from each archive
# only the members that resolve symbols left undefined by the archives before it.
# An archive listed ahead of the one that needs it contributes nothing. Apple's
# ld64 is more forgiving, so a wrong order builds on macOS and fails on Linux.

# Direct dependencies only, one entry per module plus the top-level binary.
# Everything below is derived from these, so this is the only list to maintain.
# A module with no dependencies still needs an empty entry, to register it.
MODULE_DEPS_penguinx :=				config/parsing
MODULE_DEPS_server :=				config/parsing \
									messages/request \
									messages/response
MODULE_DEPS_config/parsing :=		config/lexing \
									utils
MODULE_DEPS_config/lexing :=		utils
MODULE_DEPS_messages/request :=
MODULE_DEPS_messages/response :=
MODULE_DEPS_utils :=

# Recover the module names from the declarations above.
MODULE_LIST :=						$(patsubst MODULE_DEPS_%,%, \
										$(filter MODULE_DEPS_%,$(.VARIABLES)))

ifeq ($(filter $(NAME),$(MODULE_LIST)),)
$(error No MODULE_DEPS_$(NAME) entry in $(lastword $(MAKEFILE_LIST)); \
        every module must declare its dependencies there, even if it has none)
endif

# Topological sort, Kahn's algorithm with the outer loop unrolled: each round
# emits every module whose dependencies have all been emitted already, so a
# module can never precede something it needs. One round per module is always
# enough, and later rounds simply emit nothing once the graph is exhausted.
# MODULE_DEPS_FIRST is fixed for the duration of a round, which is what keeps
# two modules from being emitted in the same round as something they depend on.
MODULE_DEPS_FIRST :=
$(foreach _,$(MODULE_LIST), \
	$(eval MODULE_DEPS_FIRST += $(foreach MODULE, \
		$(filter-out $(MODULE_DEPS_FIRST),$(MODULE_LIST)), \
		$(if $(filter-out $(MODULE_DEPS_FIRST),$(MODULE_DEPS_$(MODULE))),, \
			$(MODULE)))))

# That order runs dependencies first; the link line needs the reverse.
MODULE_ORDER :=
$(foreach MODULE,$(MODULE_DEPS_FIRST), \
	$(eval MODULE_ORDER := $(MODULE) $(MODULE_ORDER)))

# Walk the sorted graph once, keeping the modules this one actually reaches. A
# module is wanted if NAME depends on it directly, or if a module already kept
# depends on it; because the walk is in dependency order, anything that could
# have added a module to the wanted set has been seen by the time it is reached.
MODULE_WANTED :=					$(MODULE_DEPS_$(NAME))
MODULES :=
$(foreach MODULE,$(MODULE_ORDER), \
	$(if $(filter $(MODULE),$(MODULE_WANTED)), \
		$(eval MODULES += $(MODULE)) \
		$(eval MODULE_WANTED += $(MODULE_DEPS_$(MODULE)))))
MODULES :=							$(strip $(MODULES))
