#pragma once

template<class Controller_type>
class Mock_signal_controller_factory {

public: 

    using Signal_controller_type = std::shared_ptr<Controller_type>;

    MOCK_METHOD(Signal_controller_type, create, 
        (const std::vector<int>& signals));

};