# 031 — Refresh the pattern view after a queued switch

When the main Pattern screen stayed open during a queued pattern change, the
transport state changed correctly but the header and step cells still showed
the old pattern. The incremental playhead redraw was intentionally insufficient
because it only moves the indicator.

At a pattern boundary the view now redraws the header, beat markers, track cells
and footer without clearing the entire display. Ordinary steps continue to use
the small playhead-only redraw path.

Verification: `just build-sequencer` succeeds. On hardware, keep the Pattern
screen open while queuing another pattern and confirm both `PATTERN <slot>` and
the step contents change at the boundary.
