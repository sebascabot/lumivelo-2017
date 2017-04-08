# DC/DC converter

## LED need 5V @ 10A

  Deux options
  1. Utiliser un module synchro
  2. À partir des bornes de la pile (6V, 12V ou 24V) utiliser un Stepdown (Buck) converter.

[https://www.aliexpress.com/item/DC-DC-Step-Down-Synchronous-rectification-Adjustable-Supply-Power-Module-with-LED-indicator-Button-Switch-4/995963510.html]
[https://www.aliexpress.com/item/RD-LED-Driver-12V-5V-2A-DC-DC-constant-voltage-constant-current-adjustable-buck-converter-step/1146672451.html]

## ESP need 3.3V @ 0.5A

  Deux options
  1. À partir du 5V des LED utiliser un LOD pour avoir le 3.3V
  2. À partir des bornes de la pile (6V, 12V ou 24V) utiliser un Stepdown (Buck) converter.

###Stepdown (Buck)

  DC-DC Stepdown(Buck)
  Très bon rendement, mais un peu plus dispendieux

###LDO

  Très simple à mettre en oeuvre.
  Rendement acceptable lorsque la tension d'entrée est proche de la tension de sortie.
  Dans notre cas, passage de de 5V à 3.3V


# Références

Tiny Stepdown buck converter
[http://www.mouser.com/ds/2/277/MP1584_r1.0-779241.pdf]

WeMos LDO: RT9013
Voir le schéma: [https://www.wemos.cc/sites/default/files/2016-09/mini_new_V2.pdf]
