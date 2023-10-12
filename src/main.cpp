//
// Created by copper_boy on 7/10/23.
//

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <engine/card/cards.hpp>
#include <engine/game/engine.hpp>
#include <glaze/glaze.hpp>
#include <restinio/all.hpp>
#include <restinio/router/boost_regex_engine.hpp>
#include <sw/redis++/redis++.h>

namespace like { namespace schemas {
namespace helpers {
struct add_player_t {
    size_t id;
    int32_t stacksize;

    struct glaze {
        using T = add_player_t;

        static constexpr auto value = glz::object("id", &T::id, "stacksize", &T::stacksize);
    };
};

struct action_t {
    int32_t amount;
    enums::action_t action;
    enums::position_t position;

    struct glaze {
        using T = action_t;

        static constexpr auto value = glz::object(
                "amount", &T::amount, "action", &T::action, "position", &T::position);
    };
};

struct cards_t {
    std::vector< std::string > hands;
    std::vector< std::string > board;

    struct glaze {
        using T = cards_t;

        static constexpr auto value = glz::object("hands", &T::hands, "board", &T::board);
    };
};

struct player_t {
    size_t id;
    bool is_left = false;
    int32_t stack;
    int32_t behind;
    int32_t front;
    int32_t round_bet;
    enums::state_t state;


    struct glaze {
        using T = player_t;

        static constexpr auto value = glz::object(
                        "id",
                        &T::id,
                        "is_left",
                        &T::is_left,
                        "stack",
                        &T::stack,
                        "behind",
                        &T::behind,
                        "front",
                        &T::front,
                        "round_bet",
                        &T::round_bet,
                        "state",
                        &T::state);
    };
};

struct game_t {
    int32_t sb_bet;
    int32_t bb_bet;
    uint8_t bb_mult = 20;
    std::vector< player_t > players;
    enums::position_t current;
    bool on_start_all_players_are_allin = false;
    int32_t min_raise = bb_bet;
    enums::rounds_t round = enums::rounds_t::preflop;
    bool flop_dealt = false;

    struct glaze {
        using T = game_t;

        static constexpr auto value = glz::object(
                "sb_bet",
                &T::sb_bet,
                "bb_bet",
                &T::bb_bet,
                "bb_mult",
                &T::bb_mult,
                "players",
                &T::players,
                "current",
                &T::current,
                "on_start_all_players_are_allin",
                &T::on_start_all_players_are_allin,
                "min_raise",
                &T::min_raise,
                "round",
                &T::round,
                "flop_dealt",
                &T::flop_dealt);
    };
};

struct hand_t {
    uint8_t id;
    std::string hand;

    struct glaze {
        using T = hand_t;

        static constexpr auto value = glz::object("id", &T::id, "hand", &T::hand);
    };
};
} // namespace helpers

struct add_game_request {
    std::string access;
    int32_t sb_bet;
    int32_t bb_bet;
    uint8_t bb_mult = 20;
    std::vector< helpers::player_t > players;
    enums::position_t current;
    bool on_start_all_players_are_allin = false;
    int32_t min_raise = 0;
    enums::rounds_t round = enums::rounds_t::preflop;
    bool flop_dealt = false;

    struct glaze {
        using T = add_game_request;

        static constexpr auto value = glz::object(
                        "access",
                        &T::access,
                        "sb_bet",
                        &T::sb_bet,
                        "bb_bet",
                        &T::bb_bet,
                        "bb_mult",
                        &T::bb_mult,
                        "players",
                        &T::players,
                        "current",
                        &T::current,
                        "on_start_all_players_are_allin",
                        &T::on_start_all_players_are_allin,
                        "min_raise",
                        &T::min_raise,
                        "round",
                        &T::round,
                        "flop_dealt",
                        &T::flop_dealt);
    };
};

struct adjust_game_request {
    std::string access;
    bool is_new_game = false;

    struct glaze {
        using T = adjust_game_request;

        static constexpr auto value = glz::object("access", &T::access, "is_new_game", &T::is_new_game);
    };
};

struct delete_game_request {
    std::string access;

    struct glaze {
        using T = delete_game_request;

        static constexpr auto value = glz::object("access", &T::access);
    };
};

struct execute_action_request {
    std::string access;
    helpers::action_t action;

    struct glaze {
        using T = execute_action_request;

        static constexpr auto value = glz::object("access", &T::access, "action", &T::action);
    };
};

struct get_evaluation_result_request {
    helpers::cards_t cards;

    struct glaze {
        using T = get_evaluation_result_request;

        static constexpr auto value = glz::object("cards", &T::cards);
    };
};

struct get_game_request {
    std::string access;

    struct glaze {
        using T = get_game_request;

        static constexpr auto value = glz::object("access", &T::access);
    };
};

struct get_possible_actions_request {
    std::string access;

    struct glaze {
        using T = get_possible_actions_request;

        static constexpr auto value = glz::object("access", &T::access);
    };
};

struct join_to_game_request {
    std::string access;
    size_t id;
    int32_t stacksize;

    struct glaze {
        using T = join_to_game_request;

        static constexpr auto value = glz::object(
                        "access", &T::access, "id", &T::id, "stacksize", &T::stacksize);
    };
};

struct left_from_game_request {
    std::string access;
    enums::position_t position;

    struct glaze {
        using T = left_from_game_request;

        static constexpr auto value = glz::object("access", &T::access, "position", &T::position);
    };
};

struct set_next_game_request {
    std::string access;
    bool find_winners = false;
    std::optional< helpers::cards_t > cards;

    struct glaze {
        using T = set_next_game_request;

        static constexpr auto value = glz::object(
                        "access", &T::access, "find_winners", &T::find_winners, "cards", &T::cards);
    };
};

template < typename ResponseT >
struct application_response {
    bool ok = true;
    ResponseT result;

    struct glaze {
        using T = application_response< ResponseT >;

        static constexpr auto value = glz::object("ok", &T::ok, "result", &T::result);
    };
};

struct application_error_response {
    bool ok = false;
    std::string error;

    struct glaze {
        using T = application_error_response;

        static constexpr auto value = glz::object("ok", &T::ok, "error", &T::error);
    };
};

using root_response = application_response< bool >;
using add_game_response = root_response;
using adjust_game_response = root_response;
using delete_game_response = root_response;
using execute_action_response = root_response;
using get_evaluation_result_response = application_response< std::vector< helpers::hand_t > >;
using get_game_response = application_response< helpers::game_t >;
using get_possible_actions_response = application_response< std::vector< helpers::action_t > >;
using join_to_game_response = application_response< helpers::game_t >;
using left_from_game_response = root_response;
using set_next_game_response = root_response;
}} // namespace like::schemas

namespace like { namespace utils {
struct unique_t {
public:
    explicit unique_t(uint8_t from) : current(from) {
    }

    uint8_t operator()() {
        return current++;
    }

private:
    uint8_t current = 0;
};

auto generator(uint8_t size) -> std::vector< uint8_t > {
    std::vector< uint8_t > generated(size);

    std::generate(generated.begin(), generated.end(), unique_t(0));

    return generated;
}

template < uint8_t A = 0, uint8_t B = 1 >
auto get_engine(const std::string &json) -> poker::engine< A, B > {
    auto game_json = glz::read_json< schemas::add_game_request >(json);

    std::vector< poker::player > players;
    std::transform(game_json->players.begin(),
                   game_json->players.end(),
                   std::back_inserter(players),
                   [](const auto &player) -> poker::player {
                       return poker::player(
                                       player.id,
                                       player.is_left,
                                       player.stack,
                                       player.behind,
                                       player.front,
                                       player.round_bet,
                                       player.state);
                   });

    return poker::engine< A, B >(
                    game_json->sb_bet,
                    game_json->bb_bet,
                    game_json->bb_mult,
                    players,
                    game_json->current,
                    game_json->on_start_all_players_are_allin,
                    game_json->min_raise,
                    game_json->round,
                    game_json->flop_dealt);
}

template < uint8_t A = 0, uint8_t B = 1 >
auto engine_as_game(const poker::engine< A, B > &engine) -> schemas::helpers::game_t {
    std::vector< schemas::helpers::player_t > players;
    std::transform(engine.players.begin(),
                   engine.players.end(),
                   std::back_inserter(players),
                   [](const auto &player) -> schemas::helpers::player_t {
                       return schemas::helpers::player_t(
                                       player.id,
                                       player.is_left,
                                       player.stack,
                                       player.behind,
                                       player.front,
                                       player.round_bet,
                                       player.state);
                   });

    return schemas::helpers::game_t(
                    engine.get_sb_bet(),
                    engine.get_bb_bet(),
                    engine.get_bb_mult(),
                    players,
                    engine.get_current_player(),
                    engine.get_on_start_all_players_are_allin(),
                    engine.get_min_raise(),
                    engine.get_round(),
                    engine.get_flop_dealt());
}
}} // namespace like::utils

namespace like { namespace api { namespace http {
auto add_game_route(const restinio::request_handle_t &request, sw::redis::Redis &redis)
                -> restinio::request_handling_status_t {
    auto json = glz::read_json< schemas::add_game_request >(request->body());
    if (redis.get("game_" + json->access)) {
        throw std::runtime_error("Game in use");
    }

    auto engine = utils::get_engine<>(glz::write_json(json));
    redis.set("game_" + json->access, glz::write_json(utils::engine_as_game(engine)));

    auto response = schemas::add_game_response(true, true);
    return request->create_response(restinio::status_ok())
                    .append_header(restinio::http_field::content_type, "application/json")
                    .set_body(glz::write_json(response))
                    .done();
}

auto adjust_game_route(const restinio::request_handle_t &request, sw::redis::Redis &redis)
                -> restinio::request_handling_status_t {
    auto json = glz::read_json< schemas::adjust_game_request >(request->body());
    if (!redis.get("game_" + json->access)) {
        throw std::runtime_error("Game not found");
    }

    auto engine = utils::get_engine<>(glz::write_json(json));
    engine.adjust(json->is_new_game);
    redis.set("game_" + json->access, glz::write_json(utils::engine_as_game(engine)));

    auto response = schemas::adjust_game_response(true, true);
    return request->create_response(restinio::status_ok())
                    .append_header(restinio::http_field::content_type, "application/json")
                    .set_body(glz::write_json(response))
                    .done();
}

auto delete_game_route(const restinio::request_handle_t &request, sw::redis::Redis &redis)
                -> restinio::request_handling_status_t {
    auto json = glz::read_json< schemas::delete_game_request >(request->body());

    redis.del("game_" + json->access);

    auto response = schemas::delete_game_response(true, true);
    return request->create_response(restinio::status_ok())
                    .append_header(restinio::http_field::content_type, "application/json")
                    .set_body(glz::write_json(response))
                    .done();
}

auto execute_action_route(const restinio::request_handle_t &request, sw::redis::Redis &redis)
                -> restinio::request_handling_status_t {
    auto json = glz::read_json< schemas::execute_action_request >(request->body());

    auto game = redis.get("game_" + json->access);
    if (!game) {
        throw std::runtime_error("Game not found");
    }

    auto engine = utils::get_engine<>(game.value());
    if (engine.players.size() < constants::MIN_PLAYERS) {
        throw std::runtime_error("Fuck you");
    }

    auto action = poker::player_action(
                    json->action.amount,
                    static_cast< enums::action_t >(json->action.action),
                    static_cast< enums::position_t >(json->action.position));

    engine.execute_action(action);
    redis.set("game_" + json->access, glz::write_json(utils::engine_as_game(engine)));

    auto response = schemas::execute_action_response(true, true);
    return request->create_response(restinio::status_ok())
                    .append_header(restinio::http_field::content_type, "application/json")
                    .set_body(glz::write_json(response))
                    .done();
}

auto get_evaluation_result_route(const restinio::request_handle_t &request)
                -> restinio::request_handling_status_t {
    auto json = glz::read_json< schemas::get_evaluation_result_request >(request->body());
    if (json->cards.board.size() != 5) {
        throw std::runtime_error("Invalid board size");
    }

    schemas::get_evaluation_result_response response;
    for (const auto &[result, id] : poker::get_evaluation_result(
                         poker::cards(json->cards.board, json->cards.hands),
                         utils::generator(json->cards.hands.size()))) {
        response.result.emplace_back(id, result.as_string_long());
    }

    return request->create_response(restinio::status_ok())
                    .append_header(restinio::http_field::content_type, "application/json")
                    .set_body(glz::write_json(response))
                    .done();
}

auto get_game_route(const restinio::request_handle_t &request, sw::redis::Redis &redis)
                -> restinio::request_handling_status_t {
    auto json = glz::read_json< schemas::get_game_request >(request->body());
    auto game = redis.get("game_" + json->access);
    if (!game) {
        throw std::runtime_error("Game not found");
    }

    auto response = schemas::get_game_response(true, utils::engine_as_game(utils::get_engine<>(game.value())));
    return request->create_response(restinio::status_ok())
                    .append_header(restinio::http_field::content_type, "application/json")
                    .set_body(glz::write_json(response))
                    .done();
}

auto get_possible_actions_route(const restinio::request_handle_t &request, sw::redis::Redis &redis)
                -> restinio::request_handling_status_t {
    auto json = glz::read_json< schemas::get_possible_actions_request >(request->body());
    auto game = redis.get("game_" + json->access);
    if (!game) {
        throw std::runtime_error("Game not found");
    }

    auto engine = utils::get_engine<>(game.value());
    if (engine.players.size() < constants::MIN_PLAYERS) {
        throw std::runtime_error("Fuck you");
    }

    schemas::get_possible_actions_response response;
    for (const auto &action : engine.possible_actions()) {
        response.result.emplace_back(action.amount, action.action, action.position);
    }

    return request->create_response(restinio::status_ok())
                    .append_header(restinio::http_field::content_type, "application/json")
                    .set_body(glz::write_json(response))
                    .done();
}

auto join_to_game_route(const restinio::request_handle_t &request, sw::redis::Redis &redis)
                -> restinio::request_handling_status_t {
    auto json = glz::read_json< schemas::join_to_game_request >(request->body());
    auto game = redis.get("game_" + json->access);
    if (!game) {
        throw std::runtime_error("Game not found");
    }

    auto engine = utils::get_engine<>(game.value());
    engine.add_player_to_game(json->id, json->stacksize);

    auto engine_as_game = utils::engine_as_game(engine);
    redis.set("game_" + json->access, glz::write_json(engine_as_game));

    auto response = schemas::join_to_game_response(true, engine_as_game);
    return request->create_response(restinio::status_ok())
                    .append_header(restinio::http_field::content_type, "application/json")
                    .set_body(glz::write_json(response))
                    .done();
}

auto left_from_game_route(const restinio::request_handle_t &request, sw::redis::Redis &redis)
                -> restinio::request_handling_status_t {
    auto json = glz::read_json< schemas::left_from_game_request >(request->body());
    auto game = redis.get("game_" + json->access);
    if (!game) {
        throw std::runtime_error("Game not found");
    }
    auto engine = utils::get_engine<>(game.value());

    engine.remove_player_from_game(static_cast< uint8_t >(json->position));

    redis.set("game_" + json->access, glz::write_json(utils::engine_as_game(engine)));

    auto response = schemas::left_from_game_response(true, true);
    return request->create_response(restinio::status_ok())
                    .append_header(restinio::http_field::content_type, "application/json")
                    .set_body(glz::write_json(response))
                    .done();
}

auto set_next_game_route(const restinio::request_handle_t &request, sw::redis::Redis &redis)
                -> restinio::request_handling_status_t {
    auto json = glz::read_json< schemas::set_next_game_request >(request->body());
    auto game = redis.get("game_" + json->access);
    if (!game) {
        throw std::runtime_error("Game not found");
    }

    auto engine = utils::get_engine<>(game.value());
    if (engine.players.size() < constants::MIN_PLAYERS) {
        throw std::runtime_error("Fuck you");
    }

    std::vector< int32_t > result;
    if (engine.is_showdown()) {
        if (!json->cards) {
            throw std::runtime_error("Engine needs cards to evaluate results");
        }
        result = engine.payouts_showdown(poker::cards(json->cards->board, json->cards->hands));
    } else {
        result = engine.payouts_noshowdown();
    }

    for (uint8_t i = 0; i < result.size(); i++) {
        engine.players[i].stack += result[i];
    }

    redis.set("game_" + json->access, glz::write_json(utils::engine_as_game(engine)));

    auto response = schemas::set_next_game_response(true, true);
    return request->create_response(restinio::status_ok())
                    .append_header(restinio::http_field::content_type, "application/json")
                    .set_body(glz::write_json(response))
                    .done();
}

auto setup_evaluator_router(sw::redis::Redis &redis)
                -> std::unique_ptr< restinio::router::express_router_t< restinio::router::boost_regex_engine_t > > {
    auto evaluator_router = std::make_unique<
                    restinio::router::express_router_t< restinio::router::boost_regex_engine_t > >();

    evaluator_router->http_post(
                    "/like",
                    [](const restinio::request_handle_t &request,
                       auto params) -> restinio::request_handling_status_t {
                        auto response = schemas::root_response(true, true);

                        return request->create_response(restinio::status_ok())
                                        .append_header(restinio::http_field::content_type, "application/json")
                                        .set_body(glz::write_json(response))
                                        .done();
                    });
    evaluator_router->http_post(
                    "/like/addGame",
                    [&](const restinio::request_handle_t &request,
                        auto params) -> restinio::request_handling_status_t {
                        return add_game_route(request, redis);
                    });
    evaluator_router->http_post(
                    "/like/adjustGame",
                    [&](const restinio::request_handle_t &request,
                        auto params) -> restinio::request_handling_status_t {
                        return adjust_game_route(request, redis);
                    });
    evaluator_router->http_post(
                    "/like/deleteGame",
                    [&](const restinio::request_handle_t &request,
                        auto params) -> restinio::request_handling_status_t {
                        return delete_game_route(request, redis);
                    });
    evaluator_router->http_post(
                    "/like/executeAction",
                    [&](const restinio::request_handle_t &request,
                        auto params) -> restinio::request_handling_status_t {
                        return execute_action_route(request, redis);
                    });
    evaluator_router->http_post(
                    "/like/getEvaluationResult",
                    [](const restinio::request_handle_t &request,
                       auto params) -> restinio::request_handling_status_t {
                        return get_evaluation_result_route(request);
                    });
    evaluator_router->http_post(
                    "/like/getGame",
                    [&](const restinio::request_handle_t &request,
                        auto params) -> restinio::request_handling_status_t {
                        return get_game_route(request, redis);
                    });
    evaluator_router->http_post(
                    "/like/getPossibleActions",
                    [&](const restinio::request_handle_t &request,
                        auto params) -> restinio::request_handling_status_t {
                        return get_possible_actions_route(request, redis);
                    });
    evaluator_router->http_post(
                    "/like/joinToGame",
                    [&](const restinio::request_handle_t &request,
                        auto params) -> restinio::request_handling_status_t {
                        return join_to_game_route(request, redis);
                    });
    evaluator_router->http_post(
                    "/like/leftFromGame",
                    [&](const restinio::request_handle_t &request,
                        auto params) -> restinio::request_handling_status_t {
                        return left_from_game_route(request, redis);
                    });
    evaluator_router->http_post(
                    "/like/setNextGame",
                    [&](const restinio::request_handle_t &request,
                        auto params) -> restinio::request_handling_status_t {
                        return set_next_game_route(request, redis);
                    });
    evaluator_router->non_matched_request_handler(
                    [](const restinio::request_handle_t &request) -> restinio::request_handling_status_t {
                        schemas::application_error_response response = schemas::application_error_response(
                                        false, "BAD_REQUEST");

                        return request->create_response(restinio::status_bad_request())
                                        .append_header(restinio::http_field::content_type, "application/json")
                                        .set_body(glz::write_json(response))
                                        .done();
                    });

    return evaluator_router;
}

struct server_traits : public restinio::default_traits_t {
    using request_handler_t = restinio::router::express_router_t< restinio::router::boost_regex_engine_t >;
};
}}} // namespace like::api::http

int main(int argc, char *argv[]) {
    auto redis = sw::redis::Redis(std::getenv("REDIS_URL"));
    auto evaluator_router = like::api::http::setup_evaluator_router(redis);

    restinio::run(restinio::on_thread_pool< like::api::http::server_traits >(
                                  std::thread::hardware_concurrency())
                                  .address(std::getenv("HOSTNAME"))
                                  .port(std::atoi(std::getenv("LIKE_PORT")))
                                  .request_handler(std::move(evaluator_router)));

    return 0;
}
