#include <bolos_target.h> // we need target definitions
#if defined(TARGET_NANOX)

#include <os_io_seproxyhal.h>
#include "uiHelpers.h"

// Helper macro for better astyle formatting of UX_FLOW definitions
#define LINES(...) { __VA_ARGS__ }

// Flow paginated text
void paginated_text_confirm()
{
	TRY_CATCH_UI({
		assert_uiPaginatedText_magic();
		paginatedTextState_t* ctx = paginatedTextState;
		uiCallback_confirm(&ctx->callback);
	});
}

UX_FLOW_DEF_VALID(
        ux_display_paginated_text_flow_1_step,
        bnnn_paging,
        paginated_text_confirm(),
        LINES(
                (char *) &displayState.paginatedText.header,
                (char *) &displayState.paginatedText.fullText
        )
);

// Flow prompt
void prompt_confirm()
{
	TRY_CATCH_UI({
		assert_uiPrompt_magic();
		promptState_t* ctx = promptState;
		uiCallback_confirm(&ctx->callback);
	});
}

UX_FLOW_DEF_VALID(
        ux_display_prompt_flow_1_step,
        pbb,
        prompt_confirm(),
        LINES(
                &C_icon_validate_14,
                (char *) &displayState.paginatedText.header,
                (char *) &displayState.paginatedText.currentText,
        )
);

void prompt_reject()
{
	TRY_CATCH_UI({
		assert_uiPrompt_magic();
		uiCallback_reject(&promptState->callback);
	});
}

UX_FLOW_DEF_VALID(
        ux_display_prompt_flow_2_step,
        pbb,
        prompt_reject(),
        LINES(
                &C_icon_crossmark,
                "Reject?",
                ""
        )
);

// Flow busy
UX_FLOW_DEF_NOCB(
        ux_display_busy_flow_1_step,
        pn,
        LINES(
                &C_icon_loader,
                ""
        )
);

const ux_flow_step_t * const ux_prompt_flow [] = {
	&ux_display_prompt_flow_1_step,
	&ux_display_prompt_flow_2_step,
	FLOW_END_STEP,
};

const ux_flow_step_t * const ux_paginated_text_flow [] = {
	&ux_display_paginated_text_flow_1_step,
	FLOW_END_STEP,
};

const ux_flow_step_t * const ux_busy_flow [] = {
	&ux_display_busy_flow_1_step,
	FLOW_END_STEP,
};

void ui_displayBusy()
{
	ux_flow_init(0, ux_busy_flow, NULL);
}

void ui_displayPrompt_run()
{
	ux_flow_init(0, ux_prompt_flow, NULL);
}

void ui_displayPaginatedText_run()
{
	ux_flow_init(0, ux_paginated_text_flow, NULL);
}

#endif