#include "photon/parser.hpp"

using namespace photon;

std::vector<std::string> cssparser::short_hands(std::string value)
{
    std::vector<std::string> result;
    size_t not_found = -1;
    while (validate_file(value) != not_found)
    {
        value.erase(0, validate_file(value));
        std::string short_hand = value.substr(0, value.find(" "));
        result.push_back(short_hand);
        value.erase(0, value.find(" "));
    }

    return result;
}

std::vector<std::string> cssparser::fetch_selectors(std::string selector_string)
{
    std::vector<std::string> selectors;
    for (int i = 0; selector_string[i]; i++)
    {
        if (selector_string[i] == ' ' && selector_string[i + 1] == '+')
            selector_string.replace(i, 1, "");

        if (selector_string[i] == ' ' && i > 0)
        {
            if (selector_string[i - 1] == '+')
                selector_string.replace(i, 1, "");
        }
    }

    size_t not_found = -1;
    while (validate_file(selector_string) != not_found)
    {
        std::string selector = selector_string.substr(0, selector_string.find(' '));
        selector_string.replace(0, selector_string.find(' '), "");
	size_t not_found = -1;
        if (validate_file(selector_string) != not_found)
            selector_string.replace(0, validate_file(selector_string), "");
        selectors.push_back(selector);
    }
    return selectors;
}

std::vector<cssparser::definition> cssparser::props(std::string content)
{

    const std::unordered_map<std::string, properties> propertyLocations{
        {"align-items", properties::align_items},
        {"align-self", properties::align_self},
        {"align-tracks", properties::align_tracks},
        {"all", properties::all},
        {"animation", properties::animation},
        {"animation-delay", properties::animation_delay},
        {"animation-direction", properties::animation_direction},
        {"animation-duration", properties::animation_duration},
        {"animation-fill-mode", properties::animation_fill_mode},
        {"animation-iteration-count", properties::animation_iteration_count},
        {"animation-name", properties::animation_name},
        {"animation-play-state", properties::animation_play_state},
        {"animation-timing-function", properties::animation_timing_function},
        {"appearance", properties::appearance},
        {"aspect-ratio", properties::aspect_ratio},
        {"backdrop-filter", properties::backdrop_filter},
        {"backface-visibility", properties::backface_visibility},
        {"background", properties::background},
        {"background-attachment", properties::background_attachment},
        {"background-blend-mode", properties::background_blend_mode},
        {"background-clip", properties::background_clip},
        {"background-color", properties::background_color},
        {"background-image", properties::background_image},
        {"background-origin", properties::background_origin},
        {"background-position", properties::background_position},
        {"background-position-x", properties::background_position_x},
        {"background-position-y", properties::background_position_y},
        {"background-repeat", properties::background_repeat},
        {"background-size", properties::background_size},
        {"bleedblock-overflow", properties::bleedblock_overflow},
        {"block-size", properties::block_size},
        {"border", properties::border},
        {"border-block", properties::border_block},
        {"border-block-color", properties::border_block_color},
        {"border-block-end", properties::border_block_end},
        {"border-block-end-color", properties::border_block_end_color},
        {"border-block-end-style", properties::border_block_end_style},
        {"border-block-end-width", properties::border_block_end_width},
        {"border-block-start", properties::border_block_start},
        {"border-block-start-color", properties::border_block_start_color},
        {"border-block-start-style", properties::border_block_start_style},
        {"border-block-start-width", properties::border_block_start_width},
        {"border-block-style", properties::border_block_style},
        {"border-block-width", properties::border_block_width},
        {"border-bottom", properties::border_bottom},
        {"border-bottom-color", properties::border_bottom_color},
        {"border-bottom-left-radius", properties::border_bottom_left_radius},
        {"border-bottom-right-radius", properties::border_bottom_right_radius},
        {"border-bottom-style", properties::border_bottom_style},
        {"border-bottom-width", properties::border_bottom_width},
        {"border-collapse", properties::border_collapse},
        {"border-color", properties::border_color},
        {"border-end-end-radius", properties::border_end_end_radius},
        {"border-end-start-radius", properties::border_end_start_radius},
        {"border-image", properties::border_image},
        {"border-image-outset", properties::border_image_outset},
        {"border-image-repeat", properties::border_image_repeat},
        {"border-image-slice", properties::border_image_slice},
        {"border-image-source", properties::border_image_source},
        {"border-image-width", properties::border_image_width},
        {"border-inline", properties::border_inline},
        {"border-inline-color", properties::border_inline_color},
        {"border-inline-end", properties::border_inline_end},
        {"border-inline-end-color", properties::border_inline_end_color},
        {"border-inline-end-style", properties::border_inline_end_style},
        {"border-inline-end-width", properties::border_inline_end_width},
        {"border-inline-start", properties::border_inline_start},
        {"border-inline-start-color", properties::border_inline_start_color},
        {"border-inline-start-style", properties::border_inline_start_style},
        {"border-inline-start-width", properties::border_inline_start_width},
        {"border-inline-style", properties::border_inline_style},
        {"border-inline-width", properties::border_inline_width},
        {"border-left", properties::border_left},
        {"border-left-color", properties::border_left_color},
        {"border-left-style", properties::border_left_style},
        {"border-left-width", properties::border_left_width},
        {"border-radius", properties::border_radius},
        {"border-right", properties::border_right},
        {"border-right-color", properties::border_right_color},
        {"border-right-style", properties::border_right_style},
        {"border-right-width", properties::border_right_width},
        {"border-spacing", properties::border_spacing},
        {"border-start-end-radius", properties::border_start_end_radius},
        {"border-start-start-radius", properties::border_start_start_radius},
        {"border-style", properties::border_style},
        {"border-top", properties::border_top},
        {"border-top-color", properties::border_top_color},
        {"border-top-left-radius", properties::border_top_left_radius},
        {"border-top-right-radius", properties::border_top_right_radius},
        {"border-top-style", properties::border_top_style},
        {"border-top-width", properties::border_top_width},
        {"border-width", properties::border_width},
        {"bottom", properties::bottom},
        {"box-decoration-break", properties::box_decoration_break},
        {"box-shadow", properties::box_shadow},
        {"box-sizing", properties::box_sizing},
        {"break-after", properties::break_after},
        {"break-before", properties::break_before},
        {"break-inside", properties::break_inside},
        {"caption-side", properties::caption_side},
        {"caret-color", properties::caret_color},
        {"clear", properties::clear},
        {"clip", properties::clip},
        {"clip-path", properties::clip_path},
        {"color", properties::color},
        {"color-adjust", properties::color_adjust},
        {"column-count", properties::column_count},
        {"column-fill", properties::column_fill},
        {"column-gap", properties::column_gap},
        {"column-rule", properties::column_rule},
        {"column-rule-color", properties::column_rule_color},
        {"column-rule-style", properties::column_rule_style},
        {"column-rule-width", properties::column_rule_width},
        {"column-span", properties::column_span},
        {"column-width", properties::column_width},
        {"columns", properties::columns},
        {"contain", properties::contain},
        {"content", properties::content},
        {"content-visibility", properties::content_visibility},
        {"counter-increment", properties::counter_increment},
        {"counter-reset", properties::counter_reset},
        {"counter-set", properties::counter_set},
        {"cursor", properties::cursor},
        {"direction", properties::direction},
        {"display", properties::display},
        {"empty-cells", properties::empty_cells},
        {"fallbackfilter", properties::fallbackfilter},
        {"flex", properties::flex},
        {"flex-basis", properties::flex_basis},
        {"flex-direction", properties::flex_direction},
        {"flex-flow", properties::flex_flow},
        {"flex-grow", properties::flex_grow},
        {"flex-shrink", properties::flex_shrink},
        {"flex-wrap", properties::flex_wrap},
        {"cssFloat", properties::cssFloat},
        {"font", properties::font},
        {"font-displayfont-family", properties::font_displayfont_family},
        {"font-familyfont-feature-settings", properties::font_familyfont_feature_settings},
        {"font-feature-settingsfont-kerning", properties::font_feature_settingsfont_kerning},
        {"font-language-override", properties::font_language_override},
        {"font-optical-sizing", properties::font_optical_sizing},
        {"font-size", properties::font_size},
        {"font-size-adjust", properties::font_size_adjust},
        {"font-stretch", properties::font_stretch},
        {"font-stretchfont-style", properties::font_stretchfont_style},
        {"font-stylefont-synthesis", properties::font_stylefont_synthesis},
        {"font-variant", properties::font_variant},
        {"font-variantfont-variant-alternates", properties::font_variantfont_variant_alternates},
        {"font-variant-caps", properties::font_variant_caps},
        {"font-variant-east-asian", properties::font_variant_east_asian},
        {"font-variant-ligatures", properties::font_variant_ligatures},
        {"font-variant-numeric", properties::font_variant_numeric},
        {"font-variant-position", properties::font_variant_position},
        {"font-variation-settings", properties::font_variation_settings},
        {"font-variation-settingsfont-weight", properties::font_variation_settingsfont_weight},
        {"font-weightforced-color-adjust", properties::font_weightforced_color_adjust},
        {"gap", properties::gap},
        {"grid", properties::grid},
        {"grid-area", properties::grid_area},
        {"grid-auto-columns", properties::grid_auto_columns},
        {"grid-auto-flow", properties::grid_auto_flow},
        {"grid-auto-rows", properties::grid_auto_rows},
        {"grid-column", properties::grid_column},
        {"grid-column-end", properties::grid_column_end},
        {"grid-column-start", properties::grid_column_start},
        {"grid-row", properties::grid_row},
        {"grid-row-end", properties::grid_row_end},
        {"grid-row-start", properties::grid_row_start},
        {"grid-template", properties::grid_template},
        {"grid-template-areas", properties::grid_template_areas},
        {"grid-template-columns", properties::grid_template_columns},
        {"grid-template-rows", properties::grid_template_rows},
        {"hanging-punctuation", properties::hanging_punctuation},
        {"height", properties::height},
        {"heighthyphens", properties::heighthyphens},
        {"image-orientation", properties::image_orientation},
        {"image-rendering", properties::image_rendering},
        {"image-resolution", properties::image_resolution},
        {"inherit", properties::inherit},
        {"inheritsinitial", properties::inheritsinitial},
        {"initial-letter", properties::initial_letter},
        {"initial-letter-align", properties::initial_letter_align},
        {"initial-valueinline-size", properties::initial_valueinline_size},
        {"inset", properties::inset},
        {"inset-block", properties::inset_block},
        {"inset-block-end", properties::inset_block_end},
        {"inset-block-start", properties::inset_block_start},
        {"inset-inline", properties::inset_inline},
        {"inset-inline-end", properties::inset_inline_end},
        {"inset-inline-start", properties::inset_inline_start},
        {"isolation", properties::isolation},
        {"justify-content", properties::justify_content},
        {"justify-items", properties::justify_items},
        {"justify-self", properties::justify_self},
        {"justify-tracks", properties::justify_tracks},
        {"left", properties::left},
        {"letter-spacing", properties::letter_spacing},
        {"line-break", properties::line_break},
        {"line-clamp", properties::line_clamp},
        {"line-height", properties::line_height},
        {"line-height-step", properties::line_height_step},
        {"list-style", properties::list_style},
        {"list-style-image", properties::list_style_image},
        {"list-style-position", properties::list_style_position},
        {"list-style-type", properties::list_style_type},
        {"margin", properties::margin},
        {"margin-block", properties::margin_block},
        {"margin-block-end", properties::margin_block_end},
        {"margin-block-start", properties::margin_block_start},
        {"margin-bottom", properties::margin_bottom},
        {"margin-inline", properties::margin_inline},
        {"margin-inline-end", properties::margin_inline_end},
        {"margin-inline-start", properties::margin_inline_start},
        {"margin-left", properties::margin_left},
        {"margin-right", properties::margin_right},
        {"margin-top", properties::margin_top},
        {"margin-trim", properties::margin_trim},
        {"marksmask", properties::marksmask},
        {"mask-border", properties::mask_border},
        {"mask-border-mode", properties::mask_border_mode},
        {"mask-border-outset", properties::mask_border_outset},
        {"mask-border-repeat", properties::mask_border_repeat},
        {"mask-border-slice", properties::mask_border_slice},
        {"mask-border-source", properties::mask_border_source},
        {"mask-border-width", properties::mask_border_width},
        {"mask-clip", properties::mask_clip},
        {"mask-composite", properties::mask_composite},
        {"mask-image", properties::mask_image},
        {"mask-mode", properties::mask_mode},
        {"mask-origin", properties::mask_origin},
        {"mask-position", properties::mask_position},
        {"mask-repeat", properties::mask_repeat},
        {"mask-size", properties::mask_size},
        {"mask-type", properties::mask_type},
        {"masonry-auto-flow", properties::masonry_auto_flow},
        {"math-style", properties::math_style},
        {"max-block-size", properties::max_block_size},
        {"max-height", properties::max_height},
        {"max-heightmax-inline-size", properties::max_heightmax_inline_size},
        {"max-lines", properties::max_lines},
        {"max-width", properties::max_width},
        {"max-widthmax-zoommin-block-size", properties::max_widthmax_zoommin_block_size},
        {"min-height", properties::min_height},
        {"min-heightmin-inline-size", properties::min_heightmin_inline_size},
        {"min-width", properties::min_width},
        {"min-widthmin-zoommix-blend-mode", properties::min_widthmin_zoommix_blend_mode},
        {"negativeobject-fit", properties::negativeobject_fit},
        {"object-position", properties::object_position},
        {"offset", properties::offset},
        {"offset-anchor", properties::offset_anchor},
        {"offset-distance", properties::offset_distance},
        {"offset-path", properties::offset_path},
        {"offset-position", properties::offset_position},
        {"offset-rotate", properties::offset_rotate},
        {"opacity", properties::opacity},
        {"order", properties::order},
        {"orientationorphans", properties::orientationorphans},
        {"outline", properties::outline},
        {"outline-color", properties::outline_color},
        {"outline-offset", properties::outline_offset},
        {"outline-style", properties::outline_style},
        {"outline-width", properties::outline_width},
        {"overflow", properties::overflow},
        {"overflow-anchor", properties::overflow_anchor},
        {"overflow-block", properties::overflow_block},
        {"overflow-inline", properties::overflow_inline},
        {"overflow-wrap", properties::overflow_wrap},
        {"overflow-x", properties::overflow_x},
        {"overflow-y", properties::overflow_y},
        {"overscroll-behavior", properties::overscroll_behavior},
        {"overscroll-behavior-block", properties::overscroll_behavior_block},
        {"overscroll-behavior-inline", properties::overscroll_behavior_inline},
        {"overscroll-behavior-x", properties::overscroll_behavior_x},
        {"overscroll-behavior-y", properties::overscroll_behavior_y},
        {"Pseudo-classes", properties::Pseudo_classes},
        {"Pseudo-elements", properties::Pseudo_elements},
        {"padpadding", properties::padpadding},
        {"padding-block", properties::padding_block},
        {"padding-block-end", properties::padding_block_end},
        {"padding-block-start", properties::padding_block_start},
        {"padding-bottom", properties::padding_bottom},
        {"padding-inline", properties::padding_inline},
        {"padding-inline-end", properties::padding_inline_end},
        {"padding-inline-start", properties::padding_inline_start},
        {"padding-left", properties::padding_left},
        {"padding-right", properties::padding_right},
        {"padding-top", properties::padding_top},
        {"page-break-after", properties::page_break_after},
        {"page-break-before", properties::page_break_before},
        {"page-break-inside", properties::page_break_inside},
        {"paint-order", properties::paint_order},
        {"perspective", properties::perspective},
        {"perspective-origin", properties::perspective_origin},
        {"place-content", properties::place_content},
        {"place-items", properties::place_items},
        {"place-self", properties::place_self},
        {"pointer-events", properties::pointer_events},
        {"position", properties::position},
        {"quotes", properties::quotes},
        {"rangeresize", properties::rangeresize},
        {"revert", properties::revert},
        {"right", properties::right},
        {"rotate", properties::rotate},
        {"row-gap", properties::row_gap},
        {"ruby-align", properties::ruby_align},
        {"ruby-merge", properties::ruby_merge},
        {"ruby-position", properties::ruby_position},
        {"scale", properties::scale},
        {"scroll-behavior", properties::scroll_behavior},
        {"scroll-margin", properties::scroll_margin},
        {"scroll-margin-block", properties::scroll_margin_block},
        {"scroll-margin-block-end", properties::scroll_margin_block_end},
        {"scroll-margin-block-start", properties::scroll_margin_block_start},
        {"scroll-margin-bottom", properties::scroll_margin_bottom},
        {"scroll-margin-inline", properties::scroll_margin_inline},
        {"scroll-margin-inline-end", properties::scroll_margin_inline_end},
        {"scroll-margin-inline-start", properties::scroll_margin_inline_start},
        {"scroll-margin-left", properties::scroll_margin_left},
        {"scroll-margin-right", properties::scroll_margin_right},
        {"scroll-margin-top", properties::scroll_margin_top},
        {"scroll-padding", properties::scroll_padding},
        {"scroll-padding-block", properties::scroll_padding_block},
        {"scroll-padding-block-end", properties::scroll_padding_block_end},
        {"scroll-padding-block-start", properties::scroll_padding_block_start},
        {"scroll-padding-bottom", properties::scroll_padding_bottom},
        {"scroll-padding-inline", properties::scroll_padding_inline},
        {"scroll-padding-inline-end", properties::scroll_padding_inline_end},
        {"scroll-padding-inline-start", properties::scroll_padding_inline_start},
        {"scroll-padding-left", properties::scroll_padding_left},
        {"scroll-padding-right", properties::scroll_padding_right},
        {"scroll-padding-top", properties::scroll_padding_top},
        {"scroll-snap-align", properties::scroll_snap_align},
        {"scroll-snap-stop", properties::scroll_snap_stop},
        {"scroll-snap-type", properties::scroll_snap_type},
        {"scrollbar-color", properties::scrollbar_color},
        {"scrollbar-gutter", properties::scrollbar_gutter},
        {"scrollbar-width", properties::scrollbar_width},
        {"shape-image-threshold", properties::shape_image_threshold},
        {"shape-margin", properties::shape_margin},
        {"shape-outside", properties::shape_outside},
        {"tab-size", properties::tab_size},
        {"table-layout", properties::table_layout},
        {"text-align", properties::text_align},
        {"text-align-last", properties::text_align_last},
        {"text-combine-upright", properties::text_combine_upright},
        {"text-decoration", properties::text_decoration},
        {"text-decoration-color", properties::text_decoration_color},
        {"text-decoration-line", properties::text_decoration_line},
        {"text-decoration-skip", properties::text_decoration_skip},
        {"text-decoration-skip-ink", properties::text_decoration_skip_ink},
        {"text-decoration-style", properties::text_decoration_style},
        {"text-decoration-thickness", properties::text_decoration_thickness},
        {"text-emphasis", properties::text_emphasis},
        {"text-emphasis-color", properties::text_emphasis_color},
        {"text-emphasis-position", properties::text_emphasis_position},
        {"text-emphasis-style", properties::text_emphasis_style},
        {"text-indent", properties::text_indent},
        {"text-justify", properties::text_justify},
        {"text-orientation", properties::text_orientation},
        {"text-overflow", properties::text_overflow},
        {"text-rendering", properties::text_rendering},
        {"text-shadow", properties::text_shadow},
        {"text-size-adjust", properties::text_size_adjust},
        {"text-transform", properties::text_transform},
        {"text-underline-offset", properties::text_underline_offset},
        {"text-underline-position", properties::text_underline_position},
        {"top", properties::top},
        {"touch-action", properties::touch_action},
        {"transform", properties::transform},
        {"transform-box", properties::transform_box},
        {"transform-origin", properties::transform_origin},
        {"transform-style", properties::transform_style},
        {"transition", properties::transition},
        {"transition-delay", properties::transition_delay},
        {"transition-duration", properties::transition_duration},
        {"transition-property", properties::transition_property},
        {"transition-timing-function", properties::transition_timing_function},
        {"translate", properties::translate},
        {"unicode-bidi", properties::unicode_bidi},
        {"unicode-rangeunset", properties::unicode_rangeunset},
        {"user-select", properties::user_select},
        {"vertical-align", properties::vertical_align},
        {"viewport-fitvisibility", properties::viewport_fitvisibility},
        {"white-space", properties::white_space},
        {"widows", properties::widows},
        {"width", properties::width},
        {"widthwill-change", properties::widthwill_change},
        {"word-break", properties::word_break},
        {"word-spacing", properties::word_spacing},
        {"word-wrap", properties::word_wrap},
        {"writing-mode", properties::writing_mode},
        {"z-index", properties::z_index},
        {"zoom", properties::zoom},
    };

    //* THIS FUNCTION IS HERE TO RETURN ALL THE PROPS WITH THEIR REPECTIVE VALUES AS A MAP WITH ID AND VALUE BOTH OF TYPE STRING

    std::vector<cssparser::definition> result;
    size_t not_found = -1;

    while (validate_file(content) != not_found)
    {
        if (validate_file(content) != not_found)
            content.replace(0, validate_file(content), "");
        std::string prop = content.substr(0, content.find(':'));
        std::string value = content.substr(content.find(':') + 1, content.find(';') - content.find(':') - 1);
        std::vector<std::string> values = short_hands(value);
        value.replace(0, validate_file(value), "");
        for (int i = 0; prop[i]; i++)
        {
            if (prop[i] == '-')
                prop.replace(i, 1, "-");
        }
        auto it = propertyLocations.find(prop);
        if (it == propertyLocations.end())
            throw std::invalid_argument("No valid property named " + prop);
        result.push_back({it->second, values});
        if (validate_file(content) != not_found)
            content.erase(0, content.find(';'));
    }

    return result;
}

size_t cssparser::validate_file(std::string file)
{

    /*
    * THIS FUNCTION IS HERE TO RETURN THE INDEX OF THE FIRST CHARACTER IN THE STRING
    * IF THERE IS NO CHARACTER, IT MEANS THAT THE STRING IS EMPTY OR IT HAS JUST SPACES
    * IN THAT CASE THE FUNCTION WILL RETURN -1.
    */

    std::vector<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                                 'u', 'v', 'w', 'x', 'y', 'z'};

    std::vector<char> ints = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    for (int i = 0; file[i]; i++)
    {
        if (std::find(letters.begin(), letters.end(), file[i]) != letters.end() ||
            std::find(ints.begin(), ints.end(), file[i]) != ints.end())
            return i;
    }
    return -1;
}

std::vector<cssparser::style> cssparser::parse(std::string path)
{

    std::vector<style> result;

    std::string line;
    std::ifstream cssfile(path);
    if (!cssfile.is_open())
        throw std::invalid_argument("Unable to open file");

    cssfile.seekg(0, cssfile.end);
    size_t size = cssfile.tellg();
    cssfile.seekg(0, cssfile.beg);

    std::string file;
    file.resize(size);
    cssfile.read(&file[0], size);

    size_t not_found = -1;
    while (validate_file(file) != not_found)
    {
        size_t startingPoint = validate_file(file);

        // Remove the indentation of the file
        file.replace(0, startingPoint, "");

        std::string selector_string = file.substr(0, file.find('{'));
        for (int i = 0; selector_string[i]; i++)
        {
            if (selector_string[i] == '\n')
                selector_string.replace(i, 1, "");
        }

        std::vector<std::string> selectors = fetch_selectors(selector_string);
        size_t content_start = file.find('{');
        size_t content_end = file.find('}');

        std::string content = file.substr(content_start + 1, content_end - content_start - 1);
        std::vector<definition> prop = props(content);

        for (auto i : selectors)
        {
            if (i[0] == '.')
            {
                if (i.find('>') == std::string::npos)
                    result.push_back({{Selector::Type::className, i, Selector::Relation::children}, prop});
                else
                    result.push_back({{Selector::Type::className, i, Selector::Relation::directChildren}, prop});
            }

            else if (i[0] == '#')
            {
                if (i.find('>') == std::string::npos)
                    result.push_back({{Selector::Type::id, i, Selector::Relation::children}, prop});
                else
                    result.push_back({{Selector::Type::id, i, Selector::Relation::directChildren}, prop});
            }

            else
            {
                if (i.find('>') == std::string::npos)
                    result.push_back({{Selector::Type::tagName, i, Selector::Relation::children}, prop});
                else
                    result.push_back({{Selector::Type::tagName, i, Selector::Relation::directChildren}, prop});
            }
        }
        // result.push_back({{}, prop});
        file.replace(0, file.find('}'), "");
    }

    return result;
}
