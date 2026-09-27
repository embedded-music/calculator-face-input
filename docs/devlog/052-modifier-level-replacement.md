# Replace step levels with modifier presses

## Goal

Make accent editing direct: pressing a Calculator step with a Core modifier
should assign that intensity, not remove an already active step.

## Interaction

- an unmodified Calculator press retains the normal on/off toggle;
- A plus Calculator assigns `Weak`;
- B plus Calculator assigns `Strong`;
- A+B plus Calculator assigns `Normal`, preserving the existing ambiguity
  policy;
- assigning a level to an active cell replaces its previous level;
- assigning the same level again leaves the cell active and unchanged.

This makes it possible to audition and correct intensity without first
turning a step off and recreating it.

## Implementation

The pattern model now separates the normal toggle operation from explicit
level assignment. The command router selects assignment whenever either
modifier flag is present.

## Verification

```text
pio test -e native
just build-sequencer
```

Hardware validation remains: create a normal step, replace it with weak and
strong modifiers, repeat the same modifier, then remove it with an unmodified
press.
